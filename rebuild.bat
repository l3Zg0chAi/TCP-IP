#!/bin/bash
CURRENT_DIR="$(pwd)"
cd "$CURRENT_DIR"/build
rm -rf *
cmake -S .. -B .
cmake --build .