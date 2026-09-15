#!/bin/sh
set -e

if ! command -v doxygen >/dev/null 2>&1; then
  echo "doxygen not found. Please install doxygen to generate documentation."
  exit 1
fi

echo "Generating documentation..."
CONFIG="Doxyfile"
TMP_CONFIG=""

if grep -Eq '^HAVE_DOT[[:space:]]*=[[:space:]]*YES' "$CONFIG" \
  && ! command -v dot >/dev/null 2>&1; then
  TMP_CONFIG="/tmp/zappy-doxygen-$$.conf"
  sed 's/^HAVE_DOT[[:space:]]*=.*/HAVE_DOT               = NO/' "$CONFIG" > "$TMP_CONFIG"
  CONFIG="$TMP_CONFIG"
  echo "graphviz dot not found; generating documentation without graphs."
fi

doxygen "$CONFIG"

if [ -n "$TMP_CONFIG" ]; then
  rm -f "$TMP_CONFIG"
fi

if [ -d "docs/doxygen/html" ]; then
  echo "Documentation generated at docs/doxygen/html"
else
  echo "Doxygen completed but output directory missing"
fi
