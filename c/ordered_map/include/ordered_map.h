#ifndef ORDERED_MAP_H
#define ORDERED_MAP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct ordered_map ordered_map_t;
typedef struct ordered_map_iterator ordered_map_iterator_t;

// Key comparison function type
// Returns: < 0 if key1 < key2, 0 if key1 == key2, > 0 if key1 > key2
typedef int (*key_compare_func_t)(const void* key1, const void* key2);

// Key destructor function type (optional)
typedef void (*key_destructor_func_t)(void* key);

// Value destructor function type (optional)
typedef void (*value_destructor_func_t)(void* value);

// Key copy function type (optional)
typedef void* (*key_copy_func_t)(const void* key);

// Value copy function type (optional)
typedef void* (*value_copy_func_t)(const void* value);

// Ordered map configuration structure
typedef struct {
    key_compare_func_t key_compare;        // Required: key comparison function
    key_destructor_func_t key_destructor;  // Optional: key cleanup function
    value_destructor_func_t value_destructor; // Optional: value cleanup function
    key_copy_func_t key_copy;              // Optional: key copy function
    value_copy_func_t value_copy;          // Optional: value copy function
    size_t key_size;                       // Size of key type (0 for pointer keys)
    size_t value_size;                     // Size of value type (0 for pointer values)
} ordered_map_config_t;

// Iterator direction
typedef enum {
    ORDERED_MAP_ITER_FORWARD,
    ORDERED_MAP_ITER_BACKWARD
} ordered_map_iter_direction_t;

// Error codes
typedef enum {
    ORDERED_MAP_SUCCESS = 0,
    ORDERED_MAP_ERROR_NULL_POINTER = -1,
    ORDERED_MAP_ERROR_INVALID_CONFIG = -2,
    ORDERED_MAP_ERROR_OUT_OF_MEMORY = -3,
    ORDERED_MAP_ERROR_KEY_NOT_FOUND = -4,
    ORDERED_MAP_ERROR_ITERATOR_INVALID = -5,
    ORDERED_MAP_ERROR_ITERATOR_END = -6
} ordered_map_error_t;

// Core ordered map functions
ordered_map_t* ordered_map_create(const ordered_map_config_t* config);
void ordered_map_destroy(ordered_map_t* map);

// Basic operations - O(log n) complexity
ordered_map_error_t ordered_map_insert(ordered_map_t* map, const void* key, const void* value);
ordered_map_error_t ordered_map_remove(ordered_map_t* map, const void* key);
void* ordered_map_get(ordered_map_t* map, const void* key);
bool ordered_map_contains(ordered_map_t* map, const void* key);
bool ordered_map_empty(const ordered_map_t* map);
size_t ordered_map_size(const ordered_map_t* map);

// Advanced operations - O(log n) complexity
void* ordered_map_get_or_default(ordered_map_t* map, const void* key, const void* default_value);
ordered_map_error_t ordered_map_put_if_absent(ordered_map_t* map, const void* key, const void* value);
ordered_map_error_t ordered_map_replace(ordered_map_t* map, const void* key, const void* value);
ordered_map_error_t ordered_map_replace_if_present(ordered_map_t* map, const void* key, const void* value);

// Range operations - O(log n + k) complexity where k is result size
ordered_map_error_t ordered_map_remove_range(ordered_map_t* map, const void* start_key, const void* end_key);
size_t ordered_map_count_range(ordered_map_t* map, const void* start_key, const void* end_key);

// Min/Max operations - O(log n) complexity
void* ordered_map_min_key(const ordered_map_t* map);
void* ordered_map_max_key(const ordered_map_t* map);
void* ordered_map_min_value(const ordered_map_t* map);
void* ordered_map_max_value(const ordered_map_t* map);

// Iterator functions
ordered_map_iterator_t* ordered_map_iterator_create(ordered_map_t* map, ordered_map_iter_direction_t direction);
void ordered_map_iterator_destroy(ordered_map_iterator_t* iter);
bool ordered_map_iterator_has_next(const ordered_map_iterator_t* iter);
ordered_map_error_t ordered_map_iterator_next(ordered_map_iterator_t* iter);
void* ordered_map_iterator_key(const ordered_map_iterator_t* iter);
void* ordered_map_iterator_value(const ordered_map_iterator_t* iter);

// Iterator positioning
ordered_map_error_t ordered_map_iterator_seek(ordered_map_iterator_t* iter, const void* key);
ordered_map_error_t ordered_map_iterator_seek_first(ordered_map_iterator_t* iter);
ordered_map_error_t ordered_map_iterator_seek_last(ordered_map_iterator_t* iter);

// Utility functions
ordered_map_error_t ordered_map_clear(ordered_map_t* map);
ordered_map_error_t ordered_map_merge(ordered_map_t* dest, const ordered_map_t* src);
ordered_map_t* ordered_map_copy(const ordered_map_t* map);

// Error handling
const char* ordered_map_error_string(ordered_map_error_t error);

// Debugging and introspection
void ordered_map_print(const ordered_map_t* map, void (*key_printer)(const void*), void (*value_printer)(const void*));
bool ordered_map_validate(const ordered_map_t* map);
void ordered_map_stats(const ordered_map_t* map);

// Performance profiling
typedef struct {
    size_t insert_count;
    size_t delete_count;
    size_t search_count;
    size_t total_operations;
    double avg_insert_time;
    double avg_delete_time;
    double avg_search_time;
    size_t max_height;
    size_t current_height;
} ordered_map_stats_t;

ordered_map_stats_t ordered_map_get_stats(const ordered_map_t* map);
void ordered_map_reset_stats(ordered_map_t* map);

#ifdef __cplusplus
}
#endif

#endif // ORDERED_MAP_H
