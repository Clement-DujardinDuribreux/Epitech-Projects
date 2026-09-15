#!/bin/sh
set -e

if ! command -v doxygen >/dev/null 2>&1; then
  echo "doxygen not found. Please install doxygen to generate documentation."
  exit 1
fi

echo "Generating documentation..."
doxygen Doxyfile

if [ -d "docs/doxygen/html" ]; then
  echo "Documentation generated at docs/doxygen/html"
else
  echo "Doxygen completed but output directory missing"
fi
