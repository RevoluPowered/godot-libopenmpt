#!/bin/bash
set -e

echo "Building libopenmpt-godot with tests enabled..."
scons target=template_debug arch=x86_64 tests=yes -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

echo "Setting up test environment..."
mkdir -p tests/bin
cp demo/bin/libopenmpt.gdextension tests/bin/
cp demo/bin/*.so tests/bin/ 2>/dev/null || true
cp demo/bin/*.dll tests/bin/ 2>/dev/null || true

echo "Running tests with Godot..."
godot --headless --path tests --script test_runner.gd
