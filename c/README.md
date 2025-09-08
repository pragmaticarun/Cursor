# C Programming Tutorial

A comprehensive tutorial covering C programming language fundamentals, standard library facilities, and best practices. Each topic includes runnable examples and automated tests.

## Table of Contents

1. [Quick Start](#quick-start)
2. [Tutorial Structure](#tutorial-structure)
3. [Language Features](#language-features)
4. [Standard Library](#standard-library)
5. [Testing Framework](#testing-framework)
6. [Build System](#build-system)
7. [Code Formatting](#code-formatting)
8. [Best Practices](#best-practices)

## Quick Start

### Using Make (if available)
```bash
# Build all tutorials and tests
make all

# Run all tutorials
make run-all

# Run all tests
make test-all

# Format code
make format

# Get help
make help
```

### Using Build Script (alternative)
```bash
# Build all tutorials and tests
./build.sh build

# Run all tutorials
./build.sh tutorials

# Run all tests
./build.sh tests

# Format code
./build.sh format

# Clean build files
./build.sh clean

# Get help
./build.sh help
```

## Tutorial Structure

```
c/
├── src/                    # Tutorial source files
│   ├── 01_types.c         # Data types and conversions
│   ├── 02_control_flow.c  # Control structures
│   ├── 03_functions.c     # Functions and scope
│   ├── 04_arrays_strings.c # Arrays and string handling
│   ├── 05_pointers.c      # Pointers and memory
│   ├── 06_stdio.c         # Standard I/O
│   ├── 07_stdlib.c        # Standard library utilities
│   ├── 08_strings.c       # String functions
│   └── test_framework.c   # Testing framework
├── tests/                  # Test files
│   ├── test_types.c       # Tests for types
│   ├── test_control_flow.c # Tests for control flow
│   └── test_strings.c     # Tests for strings
├── include/                # Header files
│   └── test_framework.h   # Testing framework header
├── examples/               # Additional examples
│   └── comprehensive_example.c  # Complete integration example
├── bin/                    # Compiled binaries (generated)
├── Makefile               # Build system
├── build.sh               # Alternative build script
├── .clang-format          # Code formatting rules
└── README.md              # This file
```

## Language Features

### 1. Data Types (`01_types.c`)
- Basic integer types (`int`, `long`, `char`)
- Fixed-width integers (`int32_t`, `uint64_t`)
- Floating-point types (`float`, `double`, `long double`)
- Boolean type (`bool`)
- Type conversions and casting
- Type limits and constants

**Run:** `make run-types`

### 2. Control Flow (`02_control_flow.c`)
- Conditional statements (`if`, `else`, ternary operator)
- Switch statements
- Loops (`for`, `while`, `do-while`)
- Control statements (`break`, `continue`, `goto`)
- Nested structures

**Run:** `make run-control_flow`

### 3. Functions (`03_functions.c`)
- Function definitions and prototypes
- Parameter passing (by value and reference)
- Return values and error handling
- Static variables and scope
- Recursive functions
- Variadic functions (`stdarg.h`)
- Function pointers
- Inline functions

**Run:** `make run-functions`

### 4. Arrays and Strings (`04_arrays_strings.c`)
- Array declaration and initialization
- Array operations and manipulation
- String literals and character arrays
- Dynamic arrays with `malloc`/`realloc`
- String functions and utilities
- Memory management

**Run:** `make run-arrays_strings`

### 5. Pointers (`05_pointers.c`)
- Basic pointer operations
- Pointer arithmetic
- Dynamic memory management
- Pointer to pointer
- Function pointers
- String pointers
- Void pointers
- Const pointers
- Pointer arrays

**Run:** `make run-pointers`

## Standard Library

### 6. Standard I/O (`06_stdio.c`)
- Formatted input/output (`printf`, `scanf`)
- File operations (`fopen`, `fclose`, `fread`, `fwrite`)
- Stream operations
- Binary file handling
- File positioning
- Error handling
- Temporary files

**Run:** `make run-stdio`

### 7. Standard Library (`07_stdlib.c`)
- Memory management (`malloc`, `calloc`, `realloc`, `free`)
- String conversion functions
- Sorting and searching (`qsort`, `bsearch`)
- Random number generation
- Environment variables
- System operations
- Utility functions

**Run:** `make run-stdlib`

### 8. String Functions (`08_strings.c`)
- String length and copying
- String concatenation and comparison
- String searching and tokenization
- Character classification and conversion
- String transformation
- Memory functions (`memcpy`, `memset`, `memcmp`)

**Run:** `make run-strings`

## Comprehensive Example

### Complete Integration (`examples/comprehensive_example.c`)
A comprehensive example that demonstrates the integration of multiple C concepts:

- **Data Structures**: Custom `Student` struct with dynamic memory
- **Memory Management**: `malloc`, `realloc`, `free` with proper error handling
- **Algorithms**: Bubble sort, binary search implementation
- **File I/O**: Reading and writing structured data
- **String Processing**: Trimming, case conversion, word counting
- **Error Handling**: Comprehensive error checking and cleanup

**Run:** `gcc -std=c11 -Wall -Wextra -O2 examples/comprehensive_example.c -o comprehensive && ./comprehensive`

## Testing Framework

The tutorial includes a custom testing framework with the following features:

- Simple assertion macros (`TEST_ASSERT`, `TEST_ASSERT_EQUAL`)
- Test suites and test organization
- Pass/fail reporting
- Test summary statistics

### Running Tests

```bash
# Run all tests
make test-all

# Run specific test
make test-types
make test-control_flow
make test-strings
```

### Writing Tests

```c
#include "test_framework.h"

bool test_example(void) {
    int value = 42;
    TEST_ASSERT_EQUAL(42, value);
    TEST_ASSERT_TRUE(value > 0);
    return true;
}
```

## Build System

The Makefile provides comprehensive build targets:

### Basic Targets
```bash
make help              # Show all available targets
make all               # Build everything
make tutorials         # Build all tutorial programs
make tests             # Build all test programs
make run-all           # Run all tutorials
make test-all          # Run all tests
make run-<name>        # Run specific tutorial
make test-<name>       # Run specific test
make format            # Format all source code
make clean             # Clean generated files
```

### Advanced Targets
```bash
make advanced          # Run advanced concepts tutorials
make example           # Run comprehensive example
make debug             # Build debug versions
make sanitizer         # Build sanitizer versions
make debug-<name>      # Run debug version with gdb
make san-<name>        # Run sanitizer version
make quality           # Run comprehensive quality checks
make quality-quick     # Run quick quality checks
make memory-check      # Run memory analysis
make profile           # Run performance profiling
make docs              # Generate documentation
make install-tools     # Install development tools
```

## Code Formatting

The project uses `clang-format` with Google style guidelines:

```bash
make format            # Format all source files
```

Configuration is in `.clang-format` file.

### Advanced Formatting Tools

The `tools/` directory contains additional formatting and quality tools:

```bash
# Format with clang-format
tools/format_code.sh clang

# Format with indent
tools/format_code.sh indent

# Format with astyle
tools/format_code.sh astyle

# Run all formatters
tools/format_code.sh all

# Check code quality
tools/check_quality_fixed.sh all

# Quick quality check
tools/check_quality_fixed.sh compile style bugs
```

## Best Practices

### Memory Management
- Always check return values from `malloc`/`calloc`/`realloc`
- Free allocated memory to prevent leaks
- Use `valgrind` to detect memory issues

### Error Handling
- Check return values from library functions
- Use `perror()` for system error messages
- Handle edge cases and error conditions

### Code Quality
- Use meaningful variable and function names
- Add comments for complex logic
- Follow consistent coding style
- Use compiler warnings (`-Wall -Wextra`)

### Security
- Avoid buffer overflows with bounds checking
- Use safe string functions (`strncpy`, `snprintf`)
- Validate input data
- Be careful with user input

## Compilation Flags

The tutorial uses these compiler flags:
- `-std=c11`: Use C11 standard
- `-Wall -Wextra`: Enable warnings
- `-O2`: Optimize for performance
- `-Iinclude`: Include header directory

## Advanced Topics

### Advanced C Concepts
- **Structs and Unions**: Complex data types, memory layout, type punning
- **Enums and Bitfields**: Efficient data representation, bit manipulation
- **Advanced Memory Management**: Aligned allocation, memory pools, custom allocators
- **Performance Optimization**: SIMD instructions, cache optimization, profiling
- **Debugging and Profiling**: Memory debugging, performance analysis, error detection

### Auto-Formatters and Code Quality Tools
- **clang-format**: Automatic code formatting with Google style
- **cppcheck**: Static analysis for bugs and security issues
- **valgrind**: Memory error detection and profiling
- **AddressSanitizer**: Runtime memory error detection
- **Custom Quality Checker**: Comprehensive code analysis and reporting

### Best Practices
- **Error Handling**: Proper error codes, defensive programming
- **Memory Management**: RAII patterns, resource cleanup
- **Code Style**: Consistent formatting, naming conventions
- **Testing**: Unit tests, integration tests, test coverage
- **Documentation**: Function documentation, code comments

### Performance Optimization
- **Memory Alignment**: Cache-friendly data structures
- **SIMD Instructions**: Vectorized operations (AVX2)
- **Loop Optimization**: Unrolling, vectorization
- **Branch Prediction**: Data sorting for better prediction
- **Function Inlining**: Performance-critical functions

### Debugging
```bash
# Compile with debug info
gcc -g -std=c11 -Wall -Wextra src/01_types.c -o bin/types

# Use gdb for debugging
gdb bin/types
```

### Memory Sanitizers
```bash
# Address sanitizer
gcc -fsanitize=address -std=c11 src/01_types.c -o bin/types

# Undefined behavior sanitizer
gcc -fsanitize=undefined -std=c11 src/01_types.c -o bin/types
```

### Static Analysis
```bash
# Use clang-tidy
clang-tidy src/01_types.c

# Use cppcheck
cppcheck src/01_types.c
```

## References

- [ISO C11 Standard](http://www.open-std.org/jtc1/sc22/wg14/)
- [C FAQ](https://c-faq.com/)
- [GNU C Library Manual](https://www.gnu.org/software/libc/manual/)
- [C Programming Language (K&R)](https://en.wikipedia.org/wiki/The_C_Programming_Language)

## License

This tutorial is provided as educational material. Feel free to use and modify for learning purposes.