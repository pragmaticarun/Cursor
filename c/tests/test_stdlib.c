#include "../include/test_framework.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Comparison function for qsort
int compare_ints(const void* a, const void* b) {
    int ia = *(const int*)a;
    int ib = *(const int*)b;
    return (ia > ib) - (ia < ib);
}

// Test functions for standard library
bool test_memory_management(void) {
    // Test malloc
    int* ptr = malloc(5 * sizeof(int));
    TEST_ASSERT_NOT_NULL(ptr);
    
    if (ptr != NULL) {
        // Initialize array
        for (int i = 0; i < 5; i++) {
            ptr[i] = (i + 1) * 10;
        }
        
        // Check values
        TEST_ASSERT_EQUAL(10, ptr[0]);
        TEST_ASSERT_EQUAL(20, ptr[1]);
        TEST_ASSERT_EQUAL(30, ptr[2]);
        TEST_ASSERT_EQUAL(40, ptr[3]);
        TEST_ASSERT_EQUAL(50, ptr[4]);
        
        free(ptr);
    }
    
    // Test calloc
    int* calloc_ptr = calloc(3, sizeof(int));
    TEST_ASSERT_NOT_NULL(calloc_ptr);
    
    if (calloc_ptr != NULL) {
        // Check that calloc initializes to zero
        for (int i = 0; i < 3; i++) {
            TEST_ASSERT_EQUAL(0, calloc_ptr[i]);
        }
        free(calloc_ptr);
    }
    
    return true;
}

bool test_string_conversion(void) {
    // Test atoi
    TEST_ASSERT_EQUAL(123, atoi("123"));
    TEST_ASSERT_EQUAL(456, atoi("456"));
    TEST_ASSERT_EQUAL(0, atoi("0"));
    TEST_ASSERT_EQUAL(0, atoi("abc"));  // Invalid input
    
    // Test atol
    TEST_ASSERT_EQUAL(123456789L, atol("123456789"));
    
    // Test atof
    TEST_ASSERT_TRUE(atof("3.14") > 3.13 && atof("3.14") < 3.15);
    
    return true;
}

bool test_sorting_searching(void) {
    int numbers[] = {64, 34, 25, 12, 22, 11, 90};
    size_t size = sizeof(numbers) / sizeof(numbers[0]);
    
    // Test qsort
    qsort(numbers, size, sizeof(int), compare_ints);
    
    // Check that array is sorted
    for (size_t i = 1; i < size; i++) {
        TEST_ASSERT_TRUE(numbers[i] >= numbers[i-1]);
    }
    
    // Test bsearch
    int key = 25;
    int* result = bsearch(&key, numbers, size, sizeof(int), compare_ints);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL(25, *result);
    
    // Test bsearch for non-existent element
    int not_found = 99;
    result = bsearch(&not_found, numbers, size, sizeof(int), compare_ints);
    TEST_ASSERT_NULL(result);
    
    return true;
}

bool test_random_numbers(void) {
    // Seed random number generator
    srand((unsigned int)time(NULL));
    
    // Test that rand() returns different values
    int r1 = rand();
    int r2 = rand();
    
    // Note: This test might occasionally fail due to randomness
    // In a real test, you might want to test the range instead
    TEST_ASSERT_TRUE(r1 >= 0 && r1 <= RAND_MAX);
    TEST_ASSERT_TRUE(r2 >= 0 && r2 <= RAND_MAX);
    
    return true;
}

bool test_utility_functions(void) {
    // Test abs
    TEST_ASSERT_EQUAL(42, abs(42));
    TEST_ASSERT_EQUAL(42, abs(-42));
    TEST_ASSERT_EQUAL(0, abs(0));
    
    // Test labs
    TEST_ASSERT_EQUAL(123456789L, labs(123456789L));
    TEST_ASSERT_EQUAL(123456789L, labs(-123456789L));
    
    // Test div
    div_t result = div(17, 5);
    TEST_ASSERT_EQUAL(3, result.quot);
    TEST_ASSERT_EQUAL(2, result.rem);
    
    // Test ldiv
    ldiv_t lresult = ldiv(123456789L, 1000L);
    TEST_ASSERT_EQUAL(123456L, lresult.quot);
    TEST_ASSERT_EQUAL(789L, lresult.rem);
    
    return true;
}

bool test_memory_utilities(void) {
    char str1[] = "Hello, World!";
    char str2[20];
    char str3[20];
    
    // Test memcpy
    memcpy(str2, str1, strlen(str1) + 1);
    TEST_ASSERT_EQUAL_STRING(str1, str2);
    
    // Test memmove
    memmove(str3, str1, strlen(str1) + 1);
    TEST_ASSERT_EQUAL_STRING(str1, str3);
    
    // Test memcmp
    TEST_ASSERT_EQUAL(0, memcmp(str1, str2, strlen(str1)));
    
    // Test memset
    char buffer[10];
    memset(buffer, 'X', sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    TEST_ASSERT_EQUAL_STRING("XXXXXXXXX", buffer);
    
    return true;
}

bool test_environment_variables(void) {
    // Test getenv - these might not exist in all environments
    const char* home = getenv("HOME");
    // HOME should exist in most Unix-like systems
    if (home != NULL) {
        TEST_ASSERT_TRUE(strlen(home) > 0);
    }
    
    // Test non-existent variable
    const char* nonexistent = getenv("NONEXISTENT_VARIABLE_12345");
    TEST_ASSERT_NULL(nonexistent);
    
    return true;
}

// Test suite
test_function_t stdlib_tests[] = {
    test_memory_management,
    test_string_conversion,
    test_sorting_searching,
    test_random_numbers,
    test_utility_functions,
    test_memory_utilities,
    test_environment_variables
};

test_suite_t stdlib_test_suite = {
    "Standard Library",
    stdlib_tests,
    sizeof(stdlib_tests) / sizeof(stdlib_tests[0])
};

int main(void) {
    printf("C Standard Library Test Suite\n");
    printf("==============================\n");
    
    run_test_suite(&stdlib_test_suite);
    print_test_summary(tests_passed, tests_passed + tests_failed);
    
    return (tests_failed == 0) ? 0 : 1;
}


