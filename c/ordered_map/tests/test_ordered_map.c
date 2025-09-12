#define _GNU_SOURCE
#include "test_framework.h"
#include "ordered_map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Test data structures
typedef struct {
    int id;
    char name[32];
} test_data_t;

// Key comparison functions
int int_compare(const void* a, const void* b) {
    int ia = *(const int*)a;
    int ib = *(const int*)b;
    return (ia > ib) - (ia < ib);
}

int string_compare(const void* a, const void* b) {
    return strcmp((const char*)a, (const char*)b);
}

int test_data_compare(const void* a, const void* b) {
    const test_data_t* da = (const test_data_t*)a;
    const test_data_t* db = (const test_data_t*)b;
    return (da->id > db->id) - (da->id < db->id);
}

// Test data destructors
void int_destructor(void* ptr) {
    // For integer keys, we don't need to free anything
    (void)ptr;
}

void string_destructor(void* ptr) {
    free(ptr);
}

void test_data_destructor(void* ptr) {
    free(ptr);
}

// Test data copy functions
void* int_copy(const void* ptr) {
    int* copy = malloc(sizeof(int));
    if (copy) *copy = *(const int*)ptr;
    return copy;
}

void* string_copy(const void* ptr) {
    return strdup((const char*)ptr);
}

void* test_data_copy(const void* ptr) {
    test_data_t* copy = malloc(sizeof(test_data_t));
    if (copy) *copy = *(const test_data_t*)ptr;
    return copy;
}

// Test printers
void int_printer(const void* ptr) {
    printf("%d", *(const int*)ptr);
}

void string_printer(const void* ptr) {
    printf("\"%s\"", (const char*)ptr);
}

void test_data_printer(const void* ptr) {
    const test_data_t* data = (const test_data_t*)ptr;
    printf("{%d, \"%s\"}", data->id, data->name);
}

// Test basic map creation and destruction
bool test_map_creation_destruction(void) {
    ordered_map_config_t config = {
        .key_compare = int_compare,
        .key_destructor = int_destructor,
        .value_destructor = int_destructor,
        .key_size = sizeof(int),
        .value_size = sizeof(int)
    };
    
    ordered_map_t* map = ordered_map_create(&config);
    TEST_ASSERT_NOT_NULL(map);
    TEST_ASSERT_TRUE(ordered_map_empty(map));
    TEST_ASSERT_EQUAL(0, ordered_map_size(map));
    
    ordered_map_destroy(map);
    return true;
}

// Test integer key-value operations
bool test_int_operations(void) {
    ordered_map_config_t config = {
        .key_compare = int_compare,
        .key_destructor = int_destructor,
        .value_destructor = int_destructor,
        .key_size = sizeof(int),
        .value_size = sizeof(int)
    };
    
    ordered_map_t* map = ordered_map_create(&config);
    TEST_ASSERT_NOT_NULL(map);
    
    // Test insertions
    int keys[] = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    int values[] = {50, 20, 80, 10, 90, 30, 70, 40, 60};
    size_t count = sizeof(keys) / sizeof(keys[0]);
    
    for (size_t i = 0; i < count; i++) {
        ordered_map_error_t result = ordered_map_insert(map, &keys[i], &values[i]);
        TEST_ASSERT_EQUAL(ORDERED_MAP_SUCCESS, result);
    }
    
    TEST_ASSERT_EQUAL(count, ordered_map_size(map));
    TEST_ASSERT_FALSE(ordered_map_empty(map));
    
    // Test retrievals
    for (size_t i = 0; i < count; i++) {
        int* value = (int*)ordered_map_get(map, &keys[i]);
        TEST_ASSERT_NOT_NULL(value);
        TEST_ASSERT_EQUAL(values[i], *value);
        TEST_ASSERT_TRUE(ordered_map_contains(map, &keys[i]));
    }
    
    // Test non-existent key
    int non_existent = 99;
    TEST_ASSERT_NULL(ordered_map_get(map, &non_existent));
    TEST_ASSERT_FALSE(ordered_map_contains(map, &non_existent));
    
    // Test min/max
    int* min_key = (int*)ordered_map_min_key(map);
    int* max_key = (int*)ordered_map_max_key(map);
    TEST_ASSERT_NOT_NULL(min_key);
    TEST_ASSERT_NOT_NULL(max_key);
    TEST_ASSERT_EQUAL(1, *min_key);
    TEST_ASSERT_EQUAL(9, *max_key);
    
    ordered_map_destroy(map);
    return true;
}

// Test string key-value operations
bool test_string_operations(void) {
    ordered_map_config_t config = {
        .key_compare = string_compare,
        .key_destructor = string_destructor,
        .value_destructor = string_destructor,
        .key_copy = string_copy,
        .value_copy = string_copy
    };
    
    ordered_map_t* map = ordered_map_create(&config);
    TEST_ASSERT_NOT_NULL(map);
    
    // Test insertions
    const char* keys[] = {"apple", "banana", "cherry", "date", "elderberry"};
    const char* values[] = {"red", "yellow", "red", "brown", "purple"};
    size_t count = sizeof(keys) / sizeof(keys[0]);
    
    for (size_t i = 0; i < count; i++) {
        ordered_map_error_t result = ordered_map_insert(map, keys[i], values[i]);
        TEST_ASSERT_EQUAL(ORDERED_MAP_SUCCESS, result);
    }
    
    TEST_ASSERT_EQUAL(count, ordered_map_size(map));
    
    // Test retrievals
    for (size_t i = 0; i < count; i++) {
        char* value = (char*)ordered_map_get(map, keys[i]);
        TEST_ASSERT_NOT_NULL(value);
        TEST_ASSERT_EQUAL_STRING(values[i], value);
        TEST_ASSERT_TRUE(ordered_map_contains(map, keys[i]));
    }
    
    // Test min/max (lexicographic order)
    char* min_key = (char*)ordered_map_min_key(map);
    char* max_key = (char*)ordered_map_max_key(map);
    TEST_ASSERT_NOT_NULL(min_key);
    TEST_ASSERT_NOT_NULL(max_key);
    TEST_ASSERT_EQUAL_STRING("apple", min_key);
    TEST_ASSERT_EQUAL_STRING("elderberry", max_key);
    
    ordered_map_destroy(map);
    return true;
}

// Test complex data structure operations
bool test_complex_data_operations(void) {
    ordered_map_config_t config = {
        .key_compare = test_data_compare,
        .key_destructor = test_data_destructor,
        .value_destructor = test_data_destructor,
        .key_copy = test_data_copy,
        .value_copy = test_data_copy
    };
    
    ordered_map_t* map = ordered_map_create(&config);
    TEST_ASSERT_NOT_NULL(map);
    
    // Create test data
    test_data_t* key1 = malloc(sizeof(test_data_t));
    test_data_t* key2 = malloc(sizeof(test_data_t));
    test_data_t* value1 = malloc(sizeof(test_data_t));
    test_data_t* value2 = malloc(sizeof(test_data_t));
    
    key1->id = 1;
    strcpy(key1->name, "Alice");
    key2->id = 2;
    strcpy(key2->name, "Bob");
    
    value1->id = 100;
    strcpy(value1->name, "Manager");
    value2->id = 200;
    strcpy(value2->name, "Developer");
    
    // Test insertions
    ordered_map_error_t result1 = ordered_map_insert(map, key1, value1);
    ordered_map_error_t result2 = ordered_map_insert(map, key2, value2);
    TEST_ASSERT_EQUAL(ORDERED_MAP_SUCCESS, result1);
    TEST_ASSERT_EQUAL(ORDERED_MAP_SUCCESS, result2);
    
    TEST_ASSERT_EQUAL(2, ordered_map_size(map));
    
    // Test retrievals
    test_data_t* retrieved_value1 = (test_data_t*)ordered_map_get(map, key1);
    test_data_t* retrieved_value2 = (test_data_t*)ordered_map_get(map, key2);
    TEST_ASSERT_NOT_NULL(retrieved_value1);
    TEST_ASSERT_NOT_NULL(retrieved_value2);
    TEST_ASSERT_EQUAL(100, retrieved_value1->id);
    TEST_ASSERT_EQUAL(200, retrieved_value2->id);
    TEST_ASSERT_EQUAL_STRING("Manager", retrieved_value1->name);
    TEST_ASSERT_EQUAL_STRING("Developer", retrieved_value2->name);
    
    // Clean up test data
    free(key1);
    free(key2);
    free(value1);
    free(value2);
    
    ordered_map_destroy(map);
    return true;
}

// Test deletion operations
bool test_deletion_operations(void) {
    ordered_map_config_t config = {
        .key_compare = int_compare,
        .key_destructor = int_destructor,
        .value_destructor = int_destructor,
        .key_size = sizeof(int),
        .value_size = sizeof(int)
    };
    
    ordered_map_t* map = ordered_map_create(&config);
    TEST_ASSERT_NOT_NULL(map);
    
    // Insert test data
    int keys[] = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    int values[] = {50, 20, 80, 10, 90, 30, 70, 40, 60};
    size_t count = sizeof(keys) / sizeof(keys[0]);
    
    for (size_t i = 0; i < count; i++) {
        ordered_map_insert(map, &keys[i], &values[i]);
    }
    
    TEST_ASSERT_EQUAL(count, ordered_map_size(map));
    
    // Test deletion of existing key
    int key_to_delete = 5;
    ordered_map_error_t result = ordered_map_remove(map, &key_to_delete);
    TEST_ASSERT_EQUAL(ORDERED_MAP_SUCCESS, result);
    TEST_ASSERT_EQUAL(count - 1, ordered_map_size(map));
    TEST_ASSERT_FALSE(ordered_map_contains(map, &key_to_delete));
    
    // Test deletion of non-existent key
    int non_existent = 99;
    result = ordered_map_remove(map, &non_existent);
    TEST_ASSERT_EQUAL(ORDERED_MAP_ERROR_KEY_NOT_FOUND, result);
    TEST_ASSERT_EQUAL(count - 1, ordered_map_size(map));
    
    // Test deletion of all remaining keys
    for (size_t i = 0; i < count; i++) {
        if (keys[i] != key_to_delete) {
            result = ordered_map_remove(map, &keys[i]);
            TEST_ASSERT_EQUAL(ORDERED_MAP_SUCCESS, result);
        }
    }
    
    TEST_ASSERT_TRUE(ordered_map_empty(map));
    TEST_ASSERT_EQUAL(0, ordered_map_size(map));
    
    ordered_map_destroy(map);
    return true;
}

// Test iterator operations
bool test_iterator_operations(void) {
    ordered_map_config_t config = {
        .key_compare = int_compare,
        .key_destructor = int_destructor,
        .value_destructor = int_destructor,
        .key_size = sizeof(int),
        .value_size = sizeof(int)
    };
    
    ordered_map_t* map = ordered_map_create(&config);
    TEST_ASSERT_NOT_NULL(map);
    
    // Insert test data in random order
    int keys[] = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    int values[] = {50, 20, 80, 10, 90, 30, 70, 40, 60};
    size_t count = sizeof(keys) / sizeof(keys[0]);
    
    for (size_t i = 0; i < count; i++) {
        ordered_map_insert(map, &keys[i], &values[i]);
    }
    
    // Test forward iteration
    ordered_map_iterator_t* iter = ordered_map_iterator_create(map, ORDERED_MAP_ITER_FORWARD);
    TEST_ASSERT_NOT_NULL(iter);
    
    int expected_keys[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int expected_values[] = {10, 20, 30, 40, 50, 60, 70, 80, 90};
    size_t index = 0;
    
    while (ordered_map_iterator_has_next(iter)) {
        int* key = (int*)ordered_map_iterator_key(iter);
        int* value = (int*)ordered_map_iterator_value(iter);
        
        TEST_ASSERT_NOT_NULL(key);
        TEST_ASSERT_NOT_NULL(value);
        TEST_ASSERT_EQUAL(expected_keys[index], *key);
        TEST_ASSERT_EQUAL(expected_values[index], *value);
        
        ordered_map_iterator_next(iter);
        index++;
    }
    
    TEST_ASSERT_EQUAL(count, index);
    ordered_map_iterator_destroy(iter);
    
    // Test backward iteration
    iter = ordered_map_iterator_create(map, ORDERED_MAP_ITER_BACKWARD);
    TEST_ASSERT_NOT_NULL(iter);
    
    index = count - 1;
    while (ordered_map_iterator_has_next(iter)) {
        int* key = (int*)ordered_map_iterator_key(iter);
        int* value = (int*)ordered_map_iterator_value(iter);
        
        TEST_ASSERT_NOT_NULL(key);
        TEST_ASSERT_NOT_NULL(value);
        TEST_ASSERT_EQUAL(expected_keys[index], *key);
        TEST_ASSERT_EQUAL(expected_values[index], *value);
        
        ordered_map_iterator_next(iter);
        index--;
    }
    
    TEST_ASSERT_EQUAL((size_t)-1, index);
    ordered_map_iterator_destroy(iter);
    
    ordered_map_destroy(map);
    return true;
}

// Test advanced operations
bool test_advanced_operations(void) {
    ordered_map_config_t config = {
        .key_compare = int_compare,
        .key_destructor = int_destructor,
        .value_destructor = int_destructor,
        .key_size = sizeof(int),
        .value_size = sizeof(int)
    };
    
    ordered_map_t* map = ordered_map_create(&config);
    TEST_ASSERT_NOT_NULL(map);
    
    int key1 = 1, value1 = 10;
    int key2 = 2, value2 = 20;
    int default_value = 99;
    
    // Test put_if_absent
    ordered_map_error_t result = ordered_map_put_if_absent(map, &key1, &value1);
    TEST_ASSERT_EQUAL(ORDERED_MAP_SUCCESS, result);
    TEST_ASSERT_TRUE(ordered_map_contains(map, &key1));
    
    // Test put_if_absent on existing key
    int new_value = 15;
    result = ordered_map_put_if_absent(map, &key1, &new_value);
    TEST_ASSERT_EQUAL(ORDERED_MAP_SUCCESS, result);
    int* retrieved = (int*)ordered_map_get(map, &key1);
    TEST_ASSERT_EQUAL(10, *retrieved); // Should not change
    
    // Test get_or_default
    int* value = (int*)ordered_map_get_or_default(map, &key2, &default_value);
    TEST_ASSERT_EQUAL(default_value, *value);
    
    // Test replace
    result = ordered_map_replace(map, &key1, &new_value);
    TEST_ASSERT_EQUAL(ORDERED_MAP_SUCCESS, result);
    retrieved = (int*)ordered_map_get(map, &key1);
    TEST_ASSERT_EQUAL(new_value, *retrieved);
    
    // Test replace on non-existent key
    result = ordered_map_replace(map, &key2, &value2);
    TEST_ASSERT_EQUAL(ORDERED_MAP_ERROR_KEY_NOT_FOUND, result);
    
    // Test replace_if_present
    result = ordered_map_replace_if_present(map, &key1, &value1);
    TEST_ASSERT_EQUAL(ORDERED_MAP_SUCCESS, result);
    retrieved = (int*)ordered_map_get(map, &key1);
    TEST_ASSERT_EQUAL(value1, *retrieved);
    
    result = ordered_map_replace_if_present(map, &key2, &value2);
    TEST_ASSERT_EQUAL(ORDERED_MAP_SUCCESS, result); // Should succeed but not insert
    
    ordered_map_destroy(map);
    return true;
}

// Test error handling
bool test_error_handling(void) {
    // Test NULL map operations
    TEST_ASSERT_NULL(ordered_map_create(NULL));
    
    ordered_map_config_t config = {
        .key_compare = int_compare,
        .key_destructor = int_destructor,
        .value_destructor = int_destructor,
        .key_size = sizeof(int),
        .value_size = sizeof(int)
    };
    
    ordered_map_t* map = ordered_map_create(&config);
    TEST_ASSERT_NOT_NULL(map);
    
    int key = 1, value = 10;
    
    // Test NULL key/value operations
    TEST_ASSERT_EQUAL(ORDERED_MAP_ERROR_NULL_POINTER, ordered_map_insert(NULL, &key, &value));
    TEST_ASSERT_EQUAL(ORDERED_MAP_ERROR_NULL_POINTER, ordered_map_insert(map, NULL, &value));
    TEST_ASSERT_EQUAL(ORDERED_MAP_ERROR_NULL_POINTER, ordered_map_insert(map, &key, NULL));
    
    TEST_ASSERT_EQUAL(ORDERED_MAP_ERROR_NULL_POINTER, ordered_map_remove(NULL, &key));
    TEST_ASSERT_EQUAL(ORDERED_MAP_ERROR_NULL_POINTER, ordered_map_remove(map, NULL));
    
    TEST_ASSERT_NULL(ordered_map_get(NULL, &key));
    TEST_ASSERT_NULL(ordered_map_get(map, NULL));
    
    TEST_ASSERT_FALSE(ordered_map_contains(NULL, &key));
    TEST_ASSERT_FALSE(ordered_map_contains(map, NULL));
    
    // Test error strings
    TEST_ASSERT_EQUAL_STRING("Success", ordered_map_error_string(ORDERED_MAP_SUCCESS));
    TEST_ASSERT_EQUAL_STRING("Null pointer error", ordered_map_error_string(ORDERED_MAP_ERROR_NULL_POINTER));
    TEST_ASSERT_EQUAL_STRING("Key not found", ordered_map_error_string(ORDERED_MAP_ERROR_KEY_NOT_FOUND));
    
    ordered_map_destroy(map);
    return true;
}

// Test map copying
bool test_map_copying(void) {
    ordered_map_config_t config = {
        .key_compare = int_compare,
        .key_destructor = int_destructor,
        .value_destructor = int_destructor,
        .key_size = sizeof(int),
        .value_size = sizeof(int)
    };
    
    ordered_map_t* map1 = ordered_map_create(&config);
    TEST_ASSERT_NOT_NULL(map1);
    
    // Insert test data
    int keys[] = {5, 2, 8, 1, 9};
    int values[] = {50, 20, 80, 10, 90};
    size_t count = sizeof(keys) / sizeof(keys[0]);
    
    for (size_t i = 0; i < count; i++) {
        ordered_map_insert(map1, &keys[i], &values[i]);
    }
    
    // Copy map
    ordered_map_t* map2 = ordered_map_copy(map1);
    TEST_ASSERT_NOT_NULL(map2);
    TEST_ASSERT_EQUAL(ordered_map_size(map1), ordered_map_size(map2));
    
    // Verify copied data
    for (size_t i = 0; i < count; i++) {
        int* value1 = (int*)ordered_map_get(map1, &keys[i]);
        int* value2 = (int*)ordered_map_get(map2, &keys[i]);
        TEST_ASSERT_NOT_NULL(value1);
        TEST_ASSERT_NOT_NULL(value2);
        TEST_ASSERT_EQUAL(*value1, *value2);
    }
    
    // Modify original map
    int new_key = 3, new_value = 30;
    ordered_map_insert(map1, &new_key, &new_value);
    
    // Verify copy is independent
    TEST_ASSERT_FALSE(ordered_map_contains(map2, &new_key));
    TEST_ASSERT_EQUAL(count, ordered_map_size(map2));
    TEST_ASSERT_EQUAL(count + 1, ordered_map_size(map1));
    
    ordered_map_destroy(map1);
    ordered_map_destroy(map2);
    return true;
}

// Test clear operation
bool test_clear_operation(void) {
    ordered_map_config_t config = {
        .key_compare = int_compare,
        .key_destructor = int_destructor,
        .value_destructor = int_destructor,
        .key_size = sizeof(int),
        .value_size = sizeof(int)
    };
    
    ordered_map_t* map = ordered_map_create(&config);
    TEST_ASSERT_NOT_NULL(map);
    
    // Insert test data
    int keys[] = {5, 2, 8, 1, 9};
    int values[] = {50, 20, 80, 10, 90};
    size_t count = sizeof(keys) / sizeof(keys[0]);
    
    for (size_t i = 0; i < count; i++) {
        ordered_map_insert(map, &keys[i], &values[i]);
    }
    
    TEST_ASSERT_EQUAL(count, ordered_map_size(map));
    TEST_ASSERT_FALSE(ordered_map_empty(map));
    
    // Clear map
    ordered_map_error_t result = ordered_map_clear(map);
    TEST_ASSERT_EQUAL(ORDERED_MAP_SUCCESS, result);
    TEST_ASSERT_TRUE(ordered_map_empty(map));
    TEST_ASSERT_EQUAL(0, ordered_map_size(map));
    
    // Verify all keys are gone
    for (size_t i = 0; i < count; i++) {
        TEST_ASSERT_FALSE(ordered_map_contains(map, &keys[i]));
    }
    
    ordered_map_destroy(map);
    return true;
}

// Test statistics
bool test_statistics(void) {
    ordered_map_config_t config = {
        .key_compare = int_compare,
        .key_destructor = int_destructor,
        .value_destructor = int_destructor,
        .key_size = sizeof(int),
        .value_size = sizeof(int)
    };
    
    ordered_map_t* map = ordered_map_create(&config);
    TEST_ASSERT_NOT_NULL(map);
    
    // Perform operations to generate statistics
    int keys[] = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    int values[] = {50, 20, 80, 10, 90, 30, 70, 40, 60};
    size_t count = sizeof(keys) / sizeof(keys[0]);
    
    // Insert operations
    for (size_t i = 0; i < count; i++) {
        ordered_map_insert(map, &keys[i], &values[i]);
    }
    
    // Search operations
    for (size_t i = 0; i < count; i++) {
        ordered_map_get(map, &keys[i]);
        ordered_map_contains(map, &keys[i]);
    }
    
    // Delete operations
    for (size_t i = 0; i < count / 2; i++) {
        ordered_map_remove(map, &keys[i]);
    }
    
    // Get statistics
    ordered_map_stats_t stats = ordered_map_get_stats(map);
    TEST_ASSERT_TRUE(stats.insert_count > 0);
    TEST_ASSERT_TRUE(stats.search_count > 0);
    TEST_ASSERT_TRUE(stats.delete_count > 0);
    TEST_ASSERT_TRUE(stats.total_operations > 0);
    TEST_ASSERT_TRUE(stats.current_height > 0);
    
    // Test reset statistics
    ordered_map_reset_stats(map);
    stats = ordered_map_get_stats(map);
    TEST_ASSERT_EQUAL(0, stats.insert_count);
    TEST_ASSERT_EQUAL(0, stats.search_count);
    TEST_ASSERT_EQUAL(0, stats.delete_count);
    TEST_ASSERT_EQUAL(0, stats.total_operations);
    
    ordered_map_destroy(map);
    return true;
}

// Test edge cases
bool test_edge_cases(void) {
    ordered_map_config_t config = {
        .key_compare = int_compare,
        .key_destructor = int_destructor,
        .value_destructor = int_destructor,
        .key_size = sizeof(int),
        .value_size = sizeof(int)
    };
    
    ordered_map_t* map = ordered_map_create(&config);
    TEST_ASSERT_NOT_NULL(map);
    
    // Test operations on empty map
    TEST_ASSERT_TRUE(ordered_map_empty(map));
    TEST_ASSERT_EQUAL(0, ordered_map_size(map));
    TEST_ASSERT_NULL(ordered_map_min_key(map));
    TEST_ASSERT_NULL(ordered_map_max_key(map));
    TEST_ASSERT_NULL(ordered_map_min_value(map));
    TEST_ASSERT_NULL(ordered_map_max_value(map));
    
    int key = 1;
    TEST_ASSERT_NULL(ordered_map_get(map, &key));
    TEST_ASSERT_FALSE(ordered_map_contains(map, &key));
    
    // Test iterator on empty map
    ordered_map_iterator_t* iter = ordered_map_iterator_create(map, ORDERED_MAP_ITER_FORWARD);
    TEST_ASSERT_NOT_NULL(iter);
    TEST_ASSERT_FALSE(ordered_map_iterator_has_next(iter));
    ordered_map_iterator_destroy(iter);
    
    // Test single element
    int value = 10;
    ordered_map_insert(map, &key, &value);
    TEST_ASSERT_EQUAL(1, ordered_map_size(map));
    TEST_ASSERT_FALSE(ordered_map_empty(map));
    
    int* min_key = (int*)ordered_map_min_key(map);
    int* max_key = (int*)ordered_map_max_key(map);
    TEST_ASSERT_NOT_NULL(min_key);
    TEST_ASSERT_NOT_NULL(max_key);
    TEST_ASSERT_EQUAL(key, *min_key);
    TEST_ASSERT_EQUAL(key, *max_key);
    
    ordered_map_destroy(map);
    return true;
}

// Main test runner
int main(void) {
    printf("Running Ordered Map Tests\n");
    printf("========================\n\n");
    
    run_test("Map Creation and Destruction", test_map_creation_destruction);
    run_test("Integer Operations", test_int_operations);
    run_test("String Operations", test_string_operations);
    run_test("Complex Data Operations", test_complex_data_operations);
    run_test("Deletion Operations", test_deletion_operations);
    run_test("Iterator Operations", test_iterator_operations);
    run_test("Advanced Operations", test_advanced_operations);
    run_test("Error Handling", test_error_handling);
    run_test("Map Copying", test_map_copying);
    run_test("Clear Operation", test_clear_operation);
    run_test("Statistics", test_statistics);
    run_test("Edge Cases", test_edge_cases);
    
    print_test_summary(tests_passed, tests_passed + tests_failed);
    
    return (tests_failed > 0) ? 1 : 0;
}
