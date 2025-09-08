#!/bin/bash

# C Programming Tutorial Runner Script
# This script demonstrates the tutorial without requiring make

echo "C Programming Tutorial Runner"
echo "============================"
echo ""

# Check if gcc is available
if ! command -v gcc &> /dev/null; then
    echo "Error: gcc compiler not found."
    echo "Please install gcc to run this tutorial."
    echo ""
    echo "On Ubuntu/Debian: sudo apt install gcc"
    echo "On CentOS/RHEL: sudo yum install gcc"
    echo "On macOS: xcode-select --install"
    exit 1
fi

# Create bin directory
mkdir -p bin

# Compiler flags
CFLAGS="-std=c11 -Wall -Wextra -O2 -Iinclude"

echo "Compiling tutorial programs..."
echo ""

# Compile tutorial programs
for src in src/0*.c; do
    if [ -f "$src" ]; then
        name=$(basename "$src" .c)
        echo "Compiling $name..."
        gcc $CFLAGS "$src" -o "bin/$name" 2>/dev/null
        if [ $? -eq 0 ]; then
            echo "  ✓ $name compiled successfully"
        else
            echo "  ✗ $name compilation failed"
        fi
    fi
done

echo ""
echo "Compiling test programs..."
echo ""

# Compile test programs
for test in tests/test_*.c; do
    if [ -f "$test" ]; then
        name=$(basename "$test" .c)
        echo "Compiling $name..."
        gcc $CFLAGS "$test" src/test_framework.c -o "bin/$name" 2>/dev/null
        if [ $? -eq 0 ]; then
            echo "  ✓ $name compiled successfully"
        else
            echo "  ✗ $name compilation failed"
        fi
    fi
done

echo ""
echo "Running tutorial programs..."
echo ""

# Run tutorial programs
for binary in bin/0*; do
    if [ -f "$binary" ]; then
        name=$(basename "$binary")
        echo "Running $name:"
        echo "----------------------------------------"
        ./"$binary"
        echo ""
    fi
done

echo "Running test programs..."
echo ""

# Run test programs
for binary in bin/test_*; do
    if [ -f "$binary" ]; then
        name=$(basename "$binary")
        echo "Running $name:"
        echo "----------------------------------------"
        ./"$binary"
        echo ""
    fi
done

echo "Tutorial demonstration complete!"
echo ""
echo "To run individual programs:"
echo "  ./bin/01_types"
echo "  ./bin/02_control_flow"
echo "  ./bin/03_functions"
echo "  ./bin/04_arrays_strings"
echo "  ./bin/05_pointers"
echo "  ./bin/06_stdio"
echo "  ./bin/07_stdlib"
echo "  ./bin/08_strings"
echo ""
echo "To run individual tests:"
echo "  ./bin/test_types"
echo "  ./bin/test_control_flow"
echo "  ./bin/test_functions"
echo "  ./bin/test_pointers"
echo "  ./bin/test_strings"
echo "  ./bin/test_stdlib"
