#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
echo "===== CLEAN ====="
rm -rf "$SCRIPT_DIR/build"
mkdir -p "$SCRIPT_DIR/build"
echo "===== CONFIGURE ====="
cmake -S "$SCRIPT_DIR" -B "$SCRIPT_DIR/build"
echo "===== BUILD ====="
cmake --build "$SCRIPT_DIR/build"