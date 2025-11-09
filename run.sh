#!/bin/sh
set -e

PROJECT_ROOT="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$PROJECT_ROOT/build"
BINARY_PATH="$BUILD_DIR/bin/MangoMustard"

cmake -S "$PROJECT_ROOT" -B "$BUILD_DIR"
cmake --build "$BUILD_DIR"

exec "$BINARY_PATH"
