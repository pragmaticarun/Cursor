#define _GNU_SOURCE
#include "ordered_map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Example data structures
typedef struct {
    int id;
    char name[64];
    double salary;
} employee_t;

typedef struct {
    char department[32];
    int floor;
    int capacity;
} office_t;

// Comparison functions
int int_compare(const void* a, const void* b) {
    int ia = *(const int*)a;
    int ib = *(const int*)b;
    return (ia > ib) - (ia < ib);
}

int string_compare(const void* a, const void* b) {
    return strcmp((const char*)a, (const char*)b);
}

int employee_compare(const void* a, const void* b) {
    const employee_t* ea = (const employee_t*)a;
    const employee_t* eb = (const employee_t*)b;
    return (ea->id > eb->id) - (ea->id < eb->id);
}

// Destructor functions
void employee_destructor(void* ptr) {
    free(ptr);
}

void office_destructor(void* ptr) {
    free(ptr);
}

// Copy functions
void* employee_copy(const void* ptr) {
    employee_t* copy = malloc(sizeof(employee_t));
    if (copy) {
        *copy = *(const employee_t*)ptr;
    }
    return copy;
}

void* office_copy(const void* ptr) {
    office_t* copy = malloc(sizeof(office_t));
    if (copy) {
        *copy = *(const office_t*)ptr;
    }
    return copy;
}

// Printer functions
void int_printer(const void* ptr) {
    printf("%d", *(const int*)ptr);
}

void string_printer(const void* ptr) {
    printf("\"%s\"", (const char*)ptr);
}

void employee_printer(const void* ptr) {
    const employee_t* emp = (const employee_t*)ptr;
    printf("Employee{id=%d, name=\"%s\", salary=%.2f}", 
           emp->id, emp->name, emp->salary);
}

void office_printer(const void* ptr) {
    const office_t* office = (const office_t*)ptr;
    printf("Office{dept=\"%s\", floor=%d, capacity=%d}", 
           office->department, office->floor, office->capacity);
}

// Example 1: Basic integer map operations
void example_basic_operations(void) {
    printf("Example 1: Basic Integer Map Operations\n");
    printf("=======================================\n");
    
    ordered_map_config_t config = {
        .key_compare = int_compare,
        .key_destructor = NULL, // No cleanup needed for stack-allocated integers
        .value_destructor = NULL,
        .key_size = sizeof(int),
        .value_size = sizeof(int)
    };
    
    ordered_map_t* map = ordered_map_create(&config);
    if (!map) {
        printf("Failed to create map\n");
        return;
    }
    
    // Insert some data
    int keys[] = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    int values[] = {50, 20, 80, 10, 90, 30, 70, 40, 60};
    size_t count = sizeof(keys) / sizeof(keys[0]);
    
    printf("Inserting %zu key-value pairs...\n", count);
    for (size_t i = 0; i < count; i++) {
        ordered_map_error_t result = ordered_map_insert(map, &keys[i], &values[i]);
        if (result != ORDERED_MAP_SUCCESS) {
            printf("Insert failed: %s\n", ordered_map_error_string(result));
        }
    }
    
    printf("Map size: %zu\n", ordered_map_size(map));
    printf("Map empty: %s\n", ordered_map_empty(map) ? "true" : "false");
    
    // Print the map
    printf("\nMap contents (in order):\n");
    ordered_map_print(map, int_printer, int_printer);
    
    // Test search operations
    printf("\nSearch operations:\n");
    int search_key = 5;
    int* found_value = (int*)ordered_map_get(map, &search_key);
    if (found_value) {
        printf("Key %d found with value %d\n", search_key, *found_value);
    } else {
        printf("Key %d not found\n", search_key);
    }
    
    // Test min/max operations
    int* min_key = (int*)ordered_map_min_key(map);
    int* max_key = (int*)ordered_map_max_key(map);
    if (min_key && max_key) {
        printf("Min key: %d, Max key: %d\n", *min_key, *max_key);
    }
    
    // Test iterator
    printf("\nIterating through map:\n");
    ordered_map_iterator_t* iter = ordered_map_iterator_create(map, ORDERED_MAP_ITER_FORWARD);
    if (iter) {
        while (ordered_map_iterator_has_next(iter)) {
            int* key = (int*)ordered_map_iterator_key(iter);
            int* value = (int*)ordered_map_iterator_value(iter);
            printf("  %d -> %d\n", *key, *value);
            ordered_map_iterator_next(iter);
        }
        ordered_map_iterator_destroy(iter);
    }
    
    // Test deletion
    printf("\nDeleting key 5...\n");
    ordered_map_error_t result = ordered_map_remove(map, &search_key);
    if (result == ORDERED_MAP_SUCCESS) {
        printf("Key 5 deleted successfully\n");
        printf("New map size: %zu\n", ordered_map_size(map));
    } else {
        printf("Failed to delete key 5: %s\n", ordered_map_error_string(result));
    }
    
    ordered_map_destroy(map);
    printf("\n");
}

// Example 2: String-based map
void example_string_operations(void) {
    printf("Example 2: String-based Map Operations\n");
    printf("======================================\n");
    
    ordered_map_config_t config = {
        .key_compare = string_compare,
        .key_destructor = free, // Free dynamically allocated strings
        .value_destructor = free,
        .key_copy = (void*(*)(const void*))strdup, // Duplicate strings
        .value_copy = (void*(*)(const void*))strdup
    };
    
    ordered_map_t* map = ordered_map_create(&config);
    if (!map) {
        printf("Failed to create map\n");
        return;
    }
    
    // Insert string data
    const char* countries[] = {"USA", "Canada", "Mexico", "Brazil", "Argentina"};
    const char* capitals[] = {"Washington", "Ottawa", "Mexico City", "Brasilia", "Buenos Aires"};
    size_t count = sizeof(countries) / sizeof(countries[0]);
    
    printf("Inserting country-capital pairs...\n");
    for (size_t i = 0; i < count; i++) {
        ordered_map_error_t result = ordered_map_insert(map, countries[i], capitals[i]);
        if (result != ORDERED_MAP_SUCCESS) {
            printf("Insert failed: %s\n", ordered_map_error_string(result));
        }
    }
    
    printf("Map size: %zu\n", ordered_map_size(map));
    
    // Print the map
    printf("\nMap contents (in alphabetical order):\n");
    ordered_map_print(map, string_printer, string_printer);
    
    // Test search
    const char* search_country = "Canada";
    char* found_capital = (char*)ordered_map_get(map, search_country);
    if (found_capital) {
        printf("\nCapital of %s: %s\n", search_country, found_capital);
    }
    
    // Test min/max
    char* min_country = (char*)ordered_map_min_key(map);
    char* max_country = (char*)ordered_map_max_key(map);
    if (min_country && max_country) {
        printf("First country: %s, Last country: %s\n", min_country, max_country);
    }
    
    ordered_map_destroy(map);
    printf("\n");
}

// Example 3: Complex data structures
void example_complex_data(void) {
    printf("Example 3: Complex Data Structures\n");
    printf("==================================\n");
    
    ordered_map_config_t config = {
        .key_compare = employee_compare,
        .key_destructor = employee_destructor,
        .value_destructor = office_destructor,
        .key_copy = employee_copy,
        .value_copy = office_copy
    };
    
    ordered_map_t* map = ordered_map_create(&config);
    if (!map) {
        printf("Failed to create map\n");
        return;
    }
    
    // Create employee data
    employee_t* emp1 = malloc(sizeof(employee_t));
    employee_t* emp2 = malloc(sizeof(employee_t));
    employee_t* emp3 = malloc(sizeof(employee_t));
    
    emp1->id = 1001;
    strcpy(emp1->name, "Alice Johnson");
    emp1->salary = 75000.0;
    
    emp2->id = 1002;
    strcpy(emp2->name, "Bob Smith");
    emp2->salary = 82000.0;
    
    emp3->id = 1003;
    strcpy(emp3->name, "Carol Davis");
    emp3->salary = 68000.0;
    
    // Create office data
    office_t* office1 = malloc(sizeof(office_t));
    office_t* office2 = malloc(sizeof(office_t));
    office_t* office3 = malloc(sizeof(office_t));
    
    strcpy(office1->department, "Engineering");
    office1->floor = 3;
    office1->capacity = 50;
    
    strcpy(office2->department, "Marketing");
    office2->floor = 2;
    office2->capacity = 30;
    
    strcpy(office3->department, "Sales");
    office3->floor = 1;
    office3->capacity = 25;
    
    // Insert data
    printf("Inserting employee-office assignments...\n");
    ordered_map_insert(map, emp1, office1);
    ordered_map_insert(map, emp2, office2);
    ordered_map_insert(map, emp3, office3);
    
    printf("Map size: %zu\n", ordered_map_size(map));
    
    // Print the map
    printf("\nEmployee-Office assignments (by employee ID):\n");
    ordered_map_print(map, employee_printer, office_printer);
    
    // Test search
    employee_t search_emp = {1002, "", 0.0}; // Search by ID only
    office_t* found_office = (office_t*)ordered_map_get(map, &search_emp);
    if (found_office) {
        printf("\nBob Smith's office: Floor %d, %s Department (Capacity: %d)\n",
               found_office->floor, found_office->department, found_office->capacity);
    }
    
    // Test iterator
    printf("\nIterating through assignments:\n");
    ordered_map_iterator_t* iter = ordered_map_iterator_create(map, ORDERED_MAP_ITER_FORWARD);
    if (iter) {
        while (ordered_map_iterator_has_next(iter)) {
            employee_t* emp = (employee_t*)ordered_map_iterator_key(iter);
            office_t* office = (office_t*)ordered_map_iterator_value(iter);
            printf("  %s (ID: %d) -> %s (Floor %d)\n",
                   emp->name, emp->id, office->department, office->floor);
            ordered_map_iterator_next(iter);
        }
        ordered_map_iterator_destroy(iter);
    }
    
    // Clean up
    free(emp1);
    free(emp2);
    free(emp3);
    free(office1);
    free(office2);
    free(office3);
    
    ordered_map_destroy(map);
    printf("\n");
}

// Example 4: Advanced operations
void example_advanced_operations(void) {
    printf("Example 4: Advanced Operations\n");
    printf("==============================\n");
    
    ordered_map_config_t config = {
        .key_compare = int_compare,
        .key_destructor = NULL,
        .value_destructor = NULL,
        .key_size = sizeof(int),
        .value_size = sizeof(int)
    };
    
    ordered_map_t* map = ordered_map_create(&config);
    if (!map) {
        printf("Failed to create map\n");
        return;
    }
    
    // Insert some data
    int keys[] = {1, 2, 3, 4, 5};
    int values[] = {10, 20, 30, 40, 50};
    size_t count = sizeof(keys) / sizeof(keys[0]);
    
    for (size_t i = 0; i < count; i++) {
        ordered_map_insert(map, &keys[i], &values[i]);
    }
    
    printf("Initial map size: %zu\n", ordered_map_size(map));
    
    // Test put_if_absent
    int new_key = 6, new_value = 60;
    printf("\nTesting put_if_absent for key 6...\n");
    ordered_map_error_t result = ordered_map_put_if_absent(map, &new_key, &new_value);
    if (result == ORDERED_MAP_SUCCESS) {
        printf("Key 6 added (was absent)\n");
    }
    
    // Try to add existing key
    int existing_key = 1, duplicate_value = 100;
    result = ordered_map_put_if_absent(map, &existing_key, &duplicate_value);
    if (result == ORDERED_MAP_SUCCESS) {
        printf("Key 1 already exists, no change made\n");
    }
    
    // Test get_or_default
    int non_existent_key = 99;
    int default_value = 999;
    int* value = (int*)ordered_map_get_or_default(map, &non_existent_key, &default_value);
    printf("Value for non-existent key 99: %d (default)\n", *value);
    
    // Test replace
    int replace_key = 2, replace_value = 200;
    printf("\nTesting replace for key 2...\n");
    result = ordered_map_replace(map, &replace_key, &replace_value);
    if (result == ORDERED_MAP_SUCCESS) {
        int* new_val = (int*)ordered_map_get(map, &replace_key);
        printf("Key 2 replaced with value: %d\n", *new_val);
    }
    
    // Test replace_if_present
    int present_key = 3, present_value = 300;
    result = ordered_map_replace_if_present(map, &present_key, &present_value);
    if (result == ORDERED_MAP_SUCCESS) {
        int* new_val = (int*)ordered_map_get(map, &present_key);
        printf("Key 3 replaced with value: %d\n", *new_val);
    }
    
    printf("Final map size: %zu\n", ordered_map_size(map));
    
    // Test map copying
    printf("\nTesting map copying...\n");
    ordered_map_t* map_copy = ordered_map_copy(map);
    if (map_copy) {
        printf("Map copied successfully\n");
        printf("Original size: %zu, Copy size: %zu\n", 
               ordered_map_size(map), ordered_map_size(map_copy));
        
        // Verify copy is independent
        int test_key = 7, test_value = 70;
        ordered_map_insert(map, &test_key, &test_value);
        printf("After adding to original - Original size: %zu, Copy size: %zu\n",
               ordered_map_size(map), ordered_map_size(map_copy));
        
        ordered_map_destroy(map_copy);
    }
    
    ordered_map_destroy(map);
    printf("\n");
}

// Example 5: Performance demonstration
void example_performance_demo(void) {
    printf("Example 5: Performance Demonstration\n");
    printf("====================================\n");
    
    ordered_map_config_t config = {
        .key_compare = int_compare,
        .key_destructor = NULL,
        .value_destructor = NULL,
        .key_size = sizeof(int),
        .value_size = sizeof(int)
    };
    
    ordered_map_t* map = ordered_map_create(&config);
    if (!map) {
        printf("Failed to create map\n");
        return;
    }
    
    const size_t num_operations = 10000;
    printf("Performing %zu operations...\n", num_operations);
    
    clock_t start_time = clock();
    
    // Insert operations
    for (size_t i = 0; i < num_operations; i++) {
        int key = (int)i;
        int value = (int)(i * 10);
        ordered_map_insert(map, &key, &value);
    }
    
    clock_t insert_time = clock();
    printf("Insert time: %.3f seconds\n", 
           (double)(insert_time - start_time) / CLOCKS_PER_SEC);
    
    // Search operations
    for (size_t i = 0; i < num_operations; i++) {
        int key = (int)(i % 1000); // Search for existing keys
        ordered_map_get(map, &key);
    }
    
    clock_t search_time = clock();
    printf("Search time: %.3f seconds\n", 
           (double)(search_time - insert_time) / CLOCKS_PER_SEC);
    
    // Delete operations
    for (size_t i = 0; i < num_operations / 2; i++) {
        int key = (int)i;
        ordered_map_remove(map, &key);
    }
    
    clock_t end_time = clock();
    printf("Delete time: %.3f seconds\n", 
           (double)(end_time - search_time) / CLOCKS_PER_SEC);
    printf("Total time: %.3f seconds\n", 
           (double)(end_time - start_time) / CLOCKS_PER_SEC);
    
    printf("Final map size: %zu\n", ordered_map_size(map));
    
    // Print statistics
    printf("\nMap statistics:\n");
    ordered_map_stats(map);
    
    ordered_map_destroy(map);
    printf("\n");
}

// Main function
int main(void) {
    printf("Ordered Map Examples\n");
    printf("====================\n\n");
    
    // Run all examples
    example_basic_operations();
    example_string_operations();
    example_complex_data();
    example_advanced_operations();
    example_performance_demo();
    
    printf("All examples completed successfully!\n");
    return 0;
}
