#!/usr/bin/env python3
"""@file main.py
@brief Command-line entry point for the Python Zappy AI.
"""

import sys

from ai.src.agent import Agent


def usage() -> None:
    """@brief Print the accepted command-line arguments."""
    print("USAGE: ./zappy_ai -p port -n name -h machine")
    print("OPTIONS:")
    print("\t-p port\t\tport number")
    print("\t-n name\t\tname of the team")
    print("\t-h machine\tname of the machine; localhost by default")


if __name__ == "__main__":
    machine : str = "localhost"
    port : int | None = None
    name : str | None = None

    if len(sys.argv) <= 1:
        usage()
        sys.exit(84)
    for i in range(1, len(sys.argv)):
        if sys.argv[i] == "-p" and i + 1 < len(sys.argv):
            port : str = sys.argv[i + 1]
        elif sys.argv[i] == "-n" and i + 1 < len(sys.argv):
            name : str = sys.argv[i + 1]
        elif sys.argv[i] == "-h" and i + 1 < len(sys.argv):
            machine : str = sys.argv[i + 1]
    if port is None or name is None:
        usage()
        sys.exit(84)
    try:
        agent : Agent = Agent(machine, int(port), name)
        agent.connect()
        agent.run()
    except Exception:
        sys.exit(84)
