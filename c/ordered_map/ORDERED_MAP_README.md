# Ordered Map Implementation in C

A production-ready, high-performance ordered map implementation using Red-Black Trees, providing O(log n) complexity for all operations.

## Features

- **O(log n) Operations**: Insert, delete, search, and range operations
- **Type-Safe**: Generic implementation supporting any key-value types
- **Memory Management**: Automatic cleanup with custom destructors
- **Iterator Support**: Forward and backward iteration
- **Performance Monitoring**: Built-in statistics and profiling
- **Comprehensive Testing**: Extensive test suite with edge cases
- **Production Ready**: Error handling, validation, and documentation

## Table of Contents

1. [Quick Start](#quick-start)
2. [API Reference](#api-reference)
3. [Usage Examples](#usage-examples)
4. [Performance Characteristics](#performance-characteristics)
5. [Memory Management](#memory-management)
6. [Testing](#testing)
7. [Benchmarking](#benchmarking)
8. [Building](#building)
9. [Error Handling](#error-handling)
10. [Thread Safety](#thread-safety)

## Quick Start

### Basic Usage

```c
#include "ordered_map.h"

// Define comparison function
int int_compare(const void* a, const void* b) {
    int ia = *(const int*)a;
    int ib = *(const int*)b;
    return (ia > ib) - (ia < ib);
}

// Create map configuration
ordered_map_config_t config = {
    .key_compare = int_compare,
    .key_destructor = NULL,
    .value_destructor = NULL,
    .key_size = sizeof(int),
    .value_size = sizeof(int)
};

// Create and use map
ordered_map_t* map = ordered_map_create(&config);
int key = 42, value = 100;
ordered_map_insert(map, &key, &value);
int* result = (int*)ordered_map_get(map, &key);
ordered_map_destroy(map);
```

### Building and Running

```bash
# Build ordered map components
make ordered-map

# Run example
make run-ordered-map

# Run tests
make test-ordered-map

# Run benchmarks
make benchmark-ordered-map
```

## API Reference

### Core Functions

#### Map Creation and Destruction

```c
ordered_map_t* ordered_map_create(const ordered_map_config_t* config);
void ordered_map_destroy(ordered_map_t* map);
```

#### Basic Operations

```c
// Insert/Update - O(log n)
ordered_map_error_t ordered_map_insert(ordered_map_t* map, const void* key, const void* value);

// Delete - O(log n)
ordered_map_error_t ordered_map_remove(ordered_map_t* map, const void* key);

// Search - O(log n)
void* ordered_map_get(ordered_map_t* map, const void* key);
bool ordered_map_contains(ordered_map_t* map, const void* key);

// Size and emptiness
bool ordered_map_empty(const ordered_map_t* map);
size_t ordered_map_size(const ordered_map_t* map);
```

#### Advanced Operations

```c
// Conditional operations
void* ordered_map_get_or_default(ordered_map_t* map, const void* key, const void* default_value);
ordered_map_error_t ordered_map_put_if_absent(ordered_map_t* map, const void* key, const void* value);
ordered_map_error_t ordered_map_replace(ordered_map_t* map, const void* key, const void* value);
ordered_map_error_t ordered_map_replace_if_present(ordered_map_t* map, const void* key, const void* value);

// Min/Max operations - O(log n)
void* ordered_map_min_key(const ordered_map_t* map);
void* ordered_map_max_key(const ordered_map_t* map);
void* ordered_map_min_value(const ordered_map_t* map);
void* ordered_map_max_value(const ordered_map_t* map);

// Utility operations
ordered_map_error_t ordered_map_clear(ordered_map_t* map);
ordered_map_t* ordered_map_copy(const ordered_map_t* map);
```

#### Iterator Operations

```c
// Iterator creation and destruction
ordered_map_iterator_t* ordered_map_iterator_create(ordered_map_t* map, ordered_map_iter_direction_t direction);
void ordered_map_iterator_destroy(ordered_map_iterator_t* iter);

// Iterator navigation
bool ordered_map_iterator_has_next(const ordered_map_iterator_t* iter);
ordered_map_error_t ordered_map_iterator_next(ordered_map_iterator_t* iter);

// Iterator access
void* ordered_map_iterator_key(const ordered_map_iterator_t* iter);
void* ordered_map_iterator_value(const ordered_map_iterator_t* iter);

// Iterator positioning
ordered_map_error_t ordered_map_iterator_seek(ordered_map_iterator_t* iter, const void* key);
ordered_map_error_t ordered_map_iterator_seek_first(ordered_map_iterator_t* iter);
ordered_map_error_t ordered_map_iterator_seek_last(ordered_map_iterator_t* iter);
```

### Configuration Structure

```c
typedef struct {
    key_compare_func_t key_compare;        // Required: key comparison function
    key_destructor_func_t key_destructor;  // Optional: key cleanup function
    value_destructor_func_t value_destructor; // Optional: value cleanup function
    key_copy_func_t key_copy;              // Optional: key copy function
    value_copy_func_t value_copy;          // Optional: value copy function
    size_t key_size;                       // Size of key type (0 for pointer keys)
    size_t value_size;                     // Size of value type (0 for pointer values)
} ordered_map_config_t;
```

### Error Codes

```c
typedef enum {
    ORDERED_MAP_SUCCESS = 0,
    ORDERED_MAP_ERROR_NULL_POINTER = -1,
    ORDERED_MAP_ERROR_INVALID_CONFIG = -2,
    ORDERED_MAP_ERROR_OUT_OF_MEMORY = -3,
    ORDERED_MAP_ERROR_KEY_NOT_FOUND = -4,
    ORDERED_MAP_ERROR_ITERATOR_INVALID = -5,
    ORDERED_MAP_ERROR_ITERATOR_END = -6
} ordered_map_error_t;
```

## Usage Examples

### Example 1: Integer Map

```c
#include "ordered_map.h"

int int_compare(const void* a, const void* b) {
    return *(const int*)a - *(const int*)b;
}

int main(void) {
    ordered_map_config_t config = {
        .key_compare = int_compare,
        .key_size = sizeof(int),
        .value_size = sizeof(int)
    };
    
    ordered_map_t* map = ordered_map_create(&config);
    
    // Insert data
    int keys[] = {5, 2, 8, 1, 9};
    int values[] = {50, 20, 80, 10, 90};
    
    for (size_t i = 0; i < 5; i++) {
        ordered_map_insert(map, &keys[i], &values[i]);
    }
    
    // Search
    int search_key = 5;
    int* result = (int*)ordered_map_get(map, &search_key);
    if (result) {
        printf("Key %d has value %d\n", search_key, *result);
    }
    
    // Iterate
    ordered_map_iterator_t* iter = ordered_map_iterator_create(map, ORDERED_MAP_ITER_FORWARD);
    while (ordered_map_iterator_has_next(iter)) {
        int* key = (int*)ordered_map_iterator_key(iter);
        int* value = (int*)ordered_map_iterator_value(iter);
        printf("%d -> %d\n", *key, *value);
        ordered_map_iterator_next(iter);
    }
    ordered_map_iterator_destroy(iter);
    
    ordered_map_destroy(map);
    return 0;
}
```

### Example 2: String Map

```c
#include "ordered_map.h"

int string_compare(const void* a, const void* b) {
    return strcmp((const char*)a, (const char*)b);
}

int main(void) {
    ordered_map_config_t config = {
        .key_compare = string_compare,
        .key_destructor = free,
        .value_destructor = free,
        .key_copy = strdup,
        .value_copy = strdup
    };
    
    ordered_map_t* map = ordered_map_create(&config);
    
    // Insert string data
    ordered_map_insert(map, "apple", "red");
    ordered_map_insert(map, "banana", "yellow");
    ordered_map_insert(map, "cherry", "red");
    
    // Search
    char* color = (char*)ordered_map_get(map, "apple");
    if (color) {
        printf("Apple is %s\n", color);
    }
    
    ordered_map_destroy(map);
    return 0;
}
```

### Example 3: Complex Data Structures

```c
#include "ordered_map.h"

typedef struct {
    int id;
    char name[64];
    double salary;
} employee_t;

int employee_compare(const void* a, const void* b) {
    const employee_t* ea = (const employee_t*)a;
    const employee_t* eb = (const employee_t*)b;
    return ea->id - eb->id;
}

void* employee_copy(const void* ptr) {
    employee_t* copy = malloc(sizeof(employee_t));
    if (copy) *copy = *(const employee_t*)ptr;
    return copy;
}

int main(void) {
    ordered_map_config_t config = {
        .key_compare = employee_compare,
        .key_destructor = free,
        .value_destructor = free,
        .key_copy = employee_copy,
        .value_copy = employee_copy
    };
    
    ordered_map_t* map = ordered_map_create(&config);
    
    // Create employee data
    employee_t* emp = malloc(sizeof(employee_t));
    emp->id = 1001;
    strcpy(emp->name, "John Doe");
    emp->salary = 75000.0;
    
    char* department = strdup("Engineering");
    
    // Insert
    ordered_map_insert(map, emp, department);
    
    // Search
    employee_t search_emp = {1001, "", 0.0};
    char* dept = (char*)ordered_map_get(map, &search_emp);
    if (dept) {
        printf("Employee %d works in %s\n", search_emp.id, dept);
    }
    
    // Cleanup
    free(emp);
    free(department);
    ordered_map_destroy(map);
    return 0;
}
```

## Performance Characteristics

### Time Complexity

| Operation | Time Complexity | Description |
|-----------|----------------|-------------|
| Insert | O(log n) | Insert new key-value pair |
| Delete | O(log n) | Remove key-value pair |
| Search | O(log n) | Find value by key |
| Min/Max | O(log n) | Find minimum/maximum key |
| Iterator Next | O(log n) | Move to next element |
| Range Operations | O(log n + k) | k = number of elements in range |

### Space Complexity

- **Storage**: O(n) where n is the number of elements
- **Overhead**: ~40 bytes per node (Red-Black Tree node + key/value storage)
- **Memory Alignment**: Optimized for cache performance

### Performance Benchmarks

The implementation includes comprehensive benchmarks demonstrating:

- **Scalability**: Performance across different data sizes (1K to 1M elements)
- **Complexity Verification**: O(log n) behavior confirmation
- **Memory Usage**: Linear scaling with element count
- **Iterator Performance**: O(n) for full traversal
- **Stress Testing**: High-load scenarios

Run benchmarks with:
```bash
make benchmark-ordered-map
```

## Memory Management

### Automatic Cleanup

The ordered map provides automatic memory management through destructor functions:

```c
ordered_map_config_t config = {
    .key_destructor = free,        // Called when key is removed
    .value_destructor = free,      // Called when value is removed
    .key_copy = strdup,           // Called when key is copied
    .value_copy = strdup          // Called when value is copied
};
```

### Memory Leak Prevention

- All allocated memory is automatically freed when the map is destroyed
- Custom destructors are called for each key and value
- Iterator memory is managed automatically

### Memory Usage Patterns

1. **Stack-allocated keys/values**: Set `key_size`/`value_size` to actual size
2. **Heap-allocated keys/values**: Set sizes to 0, provide copy/destructor functions
3. **Pointer keys/values**: Set sizes to 0, provide appropriate functions

## Testing

### Test Suite

The implementation includes comprehensive tests covering:

- **Basic Operations**: Insert, delete, search, iteration
- **Edge Cases**: Empty maps, single elements, duplicate keys
- **Error Handling**: NULL pointers, invalid operations
- **Memory Management**: Leak detection, proper cleanup
- **Type Safety**: Different key/value types
- **Performance**: Complexity verification

### Running Tests

```bash
# Run all tests
make test-all

# Run only ordered map tests
make test-ordered-map

# Run with memory sanitizer
make memory-check
```

### Test Coverage

- **Unit Tests**: Individual function testing
- **Integration Tests**: Complete workflow testing
- **Stress Tests**: High-load scenarios
- **Memory Tests**: Leak detection and cleanup verification

## Benchmarking

### Built-in Benchmarks

The implementation includes several benchmark suites:

1. **Scalability Benchmark**: Performance across different sizes
2. **Complexity Verification**: O(log n) behavior confirmation
3. **Memory Usage**: Memory consumption analysis
4. **Iterator Performance**: Traversal speed testing
5. **Stress Testing**: High-load performance

### Custom Benchmarks

Create custom benchmarks by using the performance monitoring API:

```c
// Enable statistics
ordered_map_t* map = ordered_map_create(&config);

// Perform operations
// ... insert, delete, search operations ...

// Get statistics
ordered_map_stats_t stats = ordered_map_get_stats(map);
printf("Insert operations: %zu\n", stats.insert_count);
printf("Average insert time: %.6f seconds\n", stats.avg_insert_time);

// Reset statistics
ordered_map_reset_stats(map);
```

## Building

### Prerequisites

- GCC 4.9+ or Clang 3.4+
- Make
- C11 standard support

### Build Targets

```bash
# Build everything
make all

# Build only ordered map
make ordered-map

# Build and run example
make run-ordered-map

# Build and run tests
make test-ordered-map

# Build and run benchmarks
make benchmark-ordered-map

# Build debug versions
make debug

# Build with sanitizers
make sanitizer
```

### Compilation Flags

- **Standard**: C11 (`-std=c11`)
- **Warnings**: All warnings enabled (`-Wall -Wextra`)
- **Optimization**: O2 for performance (`-O2`)
- **Debug**: Debug symbols (`-g`) in debug builds

## Error Handling

### Error Codes

All functions return appropriate error codes:

```c
ordered_map_error_t result = ordered_map_insert(map, key, value);
if (result != ORDERED_MAP_SUCCESS) {
    printf("Error: %s\n", ordered_map_error_string(result));
}
```

### Error Recovery

- **Memory Errors**: Graceful failure with cleanup
- **Invalid Operations**: Clear error messages
- **Iterator Errors**: Safe state management

### Best Practices

1. Always check return values
2. Use error strings for debugging
3. Clean up resources on error
4. Validate inputs before operations

## Thread Safety

### Current Status

The current implementation is **not thread-safe**. Multiple threads accessing the same map simultaneously will result in undefined behavior.

### Thread Safety Considerations

- **Read Operations**: Multiple readers require external synchronization
- **Write Operations**: Must be synchronized with all other operations
- **Iterators**: Not safe for concurrent modification

### Future Enhancements

Thread-safe versions could be implemented using:
- Reader-writer locks
- Lock-free data structures
- Copy-on-write semantics

## Advanced Usage

### Custom Comparison Functions

```c
// Case-insensitive string comparison
int case_insensitive_compare(const void* a, const void* b) {
    return strcasecmp((const char*)a, (const char*)b);
}

// Custom struct comparison
int person_compare(const void* a, const void* b) {
    const person_t* pa = (const person_t*)a;
    const person_t* pb = (const person_t*)b;
    
    // Compare by last name first, then first name
    int last_cmp = strcmp(pa->last_name, pb->last_name);
    if (last_cmp != 0) return last_cmp;
    return strcmp(pa->first_name, pb->first_name);
}
```

### Performance Optimization

```c
// Pre-allocate for known size
ordered_map_t* map = ordered_map_create(&config);

// Batch operations for better performance
for (size_t i = 0; i < large_count; i++) {
    ordered_map_insert(map, &keys[i], &values[i]);
}

// Use iterators for range operations
ordered_map_iterator_t* iter = ordered_map_iterator_create(map, ORDERED_MAP_ITER_FORWARD);
// ... process range ...
ordered_map_iterator_destroy(iter);
```

### Memory Optimization

```c
// Use stack allocation for small, fixed-size data
ordered_map_config_t config = {
    .key_size = sizeof(int),      // Stack-allocated keys
    .value_size = sizeof(int),    // Stack-allocated values
    .key_destructor = NULL,       // No cleanup needed
    .value_destructor = NULL
};

// Use custom allocators for large data
void* custom_alloc(size_t size) {
    return aligned_alloc(64, size); // Cache-aligned allocation
}
```

## Contributing

### Code Style

- Follow the existing code style
- Use meaningful variable and function names
- Add comments for complex logic
- Include error handling

### Testing

- Add tests for new features
- Ensure all tests pass
- Update benchmarks if performance changes

### Documentation

- Update API documentation
- Add usage examples
- Document performance characteristics

## License

This implementation is provided as educational material. Feel free to use and modify for learning purposes.

## References

- [Red-Black Trees](https://en.wikipedia.org/wiki/Red%E2%80%93black_tree)
- [Introduction to Algorithms (CLRS)](https://mitpress.mit.edu/books/introduction-algorithms)
- [C11 Standard](https://en.cppreference.com/w/c/11)
- [Memory Management in C](https://en.cppreference.com/w/c/memory)
