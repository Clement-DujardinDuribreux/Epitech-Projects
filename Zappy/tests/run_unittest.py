#!/usr/bin/env python3
"""Run unittest with GitHub Actions annotations and a compact summary."""

from __future__ import annotations

import argparse
import importlib.util
import sys
import traceback
import unittest
from pathlib import Path


class Ansi:
    RESET = "\033[0m"
    BOLD = "\033[1m"
    CYAN = "\033[36m"
    GREEN = "\033[32m"
    YELLOW = "\033[33m"
    RED = "\033[31m"


def colorize(text: str, code: str) -> str:
    if not sys.stdout.isatty():
        return text
    return f"{code}{text}{Ansi.RESET}"


class AnnotatingTextResult(unittest.TextTestResult):
    def addFailure(self, test: unittest.case.TestCase, err) -> None:
        super().addFailure(test, err)
        self._annotate("failure", test, err)

    def addError(self, test: unittest.case.TestCase, err) -> None:
        super().addError(test, err)
        self._annotate("error", test, err)

    def _annotate(self, kind: str, test: unittest.case.TestCase, err) -> None:
        filename, line = self._test_location(test)
        message = self._first_error_line(err)
        escaped = message.replace("%", "%25").replace("\n", "%0A").replace("\r", "%0D")
        print(f"::error file={filename},line={line},title=Python unittest {kind}::{escaped}")

    @staticmethod
    def _test_location(test: unittest.case.TestCase) -> tuple[str, int]:
        method = getattr(test, test._testMethodName)
        code = method.__code__
        return code.co_filename, code.co_firstlineno

    @staticmethod
    def _first_error_line(err) -> str:
        formatted = traceback.format_exception(*err)
        for line in reversed("".join(formatted).splitlines()):
            if line.strip():
                return line.strip()
        return "unittest failure"


class AnnotatingTextRunner(unittest.TextTestRunner):
    resultclass = AnnotatingTextResult


def main() -> int:
    parser = argparse.ArgumentParser(description="Run Python unittest suite.")
    parser.add_argument("--start-directory", default="tests")
    parser.add_argument("--pattern", default="test_*.py")
    args = parser.parse_args()

    root = Path(__file__).resolve().parents[1]
    suite = unittest.TestSuite()
    start_directory = root / args.start_directory
    for index, path in enumerate(sorted(start_directory.glob(args.pattern))):
        module_name = f"_zappy_unittest_{index}_{path.stem}"
        spec = importlib.util.spec_from_file_location(module_name, path)
        if spec is None or spec.loader is None:
            raise ImportError(f"Cannot load test file: {path}")
        module = importlib.util.module_from_spec(spec)
        sys.modules[module_name] = module
        spec.loader.exec_module(module)
        suite.addTests(unittest.defaultTestLoader.loadTestsFromModule(module))
    runner = AnnotatingTextRunner(verbosity=2, buffer=True)
    result = runner.run(suite)

    print(colorize("\nPython unittest summary", Ansi.CYAN + Ansi.BOLD))
    print(f"  tests: {colorize(str(result.testsRun), Ansi.CYAN)}")
    print(f"  successes: {colorize(str(result.testsRun - len(result.failures) - len(result.errors) - len(result.skipped)), Ansi.GREEN)}")
    print(f"  failures: {colorize(str(len(result.failures)), Ansi.RED)}")
    print(f"  errors: {colorize(str(len(result.errors)), Ansi.RED)}")
    print(f"  skipped: {colorize(str(len(result.skipped)), Ansi.YELLOW)}")
    return 0 if result.wasSuccessful() else 1


if __name__ == "__main__":
    sys.exit(main())
