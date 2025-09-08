#!/bin/bash

# C Programming Tutorial Build Script
# This script builds all tutorials and tests without requiring make

set -e  # Exit on any error

# Configuration
CC="gcc"
CFLAGS="-std=c11 -Wall -Wextra -O2 -Iinclude"
SRCDIR="src"
TESTDIR="tests"
BINDIR="bin"
INCDIR="include"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if compiler is available
check_compiler() {
    if ! command -v gcc &> /dev/null; then
        print_error "gcc compiler not found. Please install gcc."
        exit 1
    fi
    print_success "gcc compiler found"
}

# Create directories
create_directories() {
    print_status "Creating directories..."
    mkdir -p "$BINDIR"
    print_success "Directories created"
}

# Build tutorial programs
build_tutorials() {
    print_status "Building tutorial programs..."
    
    local count=0
    for src_file in "$SRCDIR"/*.c; do
        if [[ -f "$src_file" ]]; then
            local basename=$(basename "$src_file" .c)
            local output="$BINDIR/$basename"
            
            print_status "Building $basename..."
            if $CC $CFLAGS "$src_file" -o "$output"; then
                print_success "Built $basename"
                ((count++))
            else
                print_error "Failed to build $basename"
            fi
        fi
    done
    
    print_success "Built $count tutorial programs"
}

# Build test programs
build_tests() {
    print_status "Building test programs..."
    
    local count=0
    for test_file in "$TESTDIR"/*.c; do
        if [[ -f "$test_file" ]]; then
            local basename=$(basename "$test_file" .c)
            local output="$BINDIR/test_$basename"
            
            print_status "Building test_$basename..."
            if $CC $CFLAGS "$test_file" "$SRCDIR/test_framework.c" -o "$output"; then
                print_success "Built test_$basename"
                ((count++))
            else
                print_error "Failed to build test_$basename"
            fi
        fi
    done
    
    print_success "Built $count test programs"
}

# Run all tutorials
run_tutorials() {
    print_status "Running all tutorial programs..."
    
    local count=0
    for tutorial in "$BINDIR"/[0-9]*; do
        if [[ -f "$tutorial" && -x "$tutorial" ]]; then
            local basename=$(basename "$tutorial")
            print_status "Running $basename..."
            echo "----------------------------------------"
            if ./"$tutorial"; then
                print_success "Completed $basename"
                ((count++))
            else
                print_error "Failed to run $basename"
            fi
            echo ""
        fi
    done
    
    print_success "Ran $count tutorial programs"
}

# Run all tests
run_tests() {
    print_status "Running all test programs..."
    
    local count=0
    local passed=0
    local failed=0
    
    for test in "$BINDIR"/test_*; do
        if [[ -f "$test" && -x "$test" ]]; then
            local basename=$(basename "$test")
            print_status "Running $basename..."
            echo "----------------------------------------"
            if ./"$test"; then
                print_success "Test $basename passed"
                ((passed++))
            else
                print_error "Test $basename failed"
                ((failed++))
            fi
            ((count++))
            echo ""
        fi
    done
    
    print_status "Test Summary: $passed passed, $failed failed out of $count tests"
    
    if [[ $failed -eq 0 ]]; then
        print_success "All tests passed! 🎉"
    else
        print_error "Some tests failed! ❌"
    fi
}

# Format code
format_code() {
    print_status "Formatting code..."
    
    if command -v clang-format &> /dev/null; then
        clang-format -i "$SRCDIR"/*.c "$TESTDIR"/*.c "$INCDIR"/*.h 2>/dev/null || true
        print_success "Code formatted"
    else
        print_warning "clang-format not found, skipping formatting"
    fi
}

# Clean build artifacts
clean() {
    print_status "Cleaning build artifacts..."
    rm -rf "$BINDIR"
    print_success "Clean complete"
}

# Show help
show_help() {
    echo "C Programming Tutorial Build Script"
    echo "==================================="
    echo ""
    echo "Usage: $0 [COMMAND]"
    echo ""
    echo "Commands:"
    echo "  build     - Build all tutorials and tests"
    echo "  tutorials - Build and run all tutorials"
    echo "  tests     - Build and run all tests"
    echo "  run       - Run all built programs"
    echo "  format    - Format source code"
    echo "  clean     - Clean build artifacts"
    echo "  help      - Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0 build      # Build everything"
    echo "  $0 tutorials  # Build and run tutorials"
    echo "  $0 tests      # Build and run tests"
    echo "  $0 clean      # Clean build files"
}

# Main function
main() {
    case "${1:-build}" in
        "build")
            check_compiler
            create_directories
            build_tutorials
            build_tests
            print_success "Build complete!"
            ;;
        "tutorials")
            check_compiler
            create_directories
            build_tutorials
            run_tutorials
            ;;
        "tests")
            check_compiler
            create_directories
            build_tests
            run_tests
            ;;
        "run")
            run_tutorials
            run_tests
            ;;
        "format")
            format_code
            ;;
        "clean")
            clean
            ;;
        "help"|"-h"|"--help")
            show_help
            ;;
        *)
            print_error "Unknown command: $1"
            show_help
            exit 1
            ;;
    esac
}

# Run main function with all arguments
main "$@"

