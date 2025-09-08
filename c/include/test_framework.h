#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Test framework macros and functions
#define TEST_ASSERT(condition) \
    do { \
        if (!(condition)) { \
            printf("FAIL: %s:%d - Assertion failed: %s\n", __FILE__, __LINE__, #condition); \
            return false; \
        } \
    } while(0)

#define TEST_ASSERT_EQUAL(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            printf("FAIL: %s:%d - Expected %d, got %d\n", __FILE__, __LINE__, (int)(expected), (int)(actual)); \
            return false; \
        } \
    } while(0)

#define TEST_ASSERT_EQUAL_STRING(expected, actual) \
    do { \
        if (strcmp((expected), (actual)) != 0) { \
            printf("FAIL: %s:%d - Expected '%s', got '%s'\n", __FILE__, __LINE__, (expected), (actual)); \
            return false; \
        } \
    } while(0)

#define TEST_ASSERT_NOT_NULL(ptr) \
    do { \
        if ((ptr) == NULL) { \
            printf("FAIL: %s:%d - Pointer is NULL\n", __FILE__, __LINE__); \
            return false; \
        } \
    } while(0)

#define TEST_ASSERT_NULL(ptr) \
    do { \
        if ((ptr) != NULL) { \
            printf("FAIL: %s:%d - Pointer is not NULL\n", __FILE__, __LINE__); \
            return false; \
        } \
    } while(0)

#define TEST_ASSERT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            printf("FAIL: %s:%d - Expected true, got false\n", __FILE__, __LINE__); \
            return false; \
        } \
    } while(0)

#define TEST_ASSERT_FALSE(condition) \
    do { \
        if (condition) { \
            printf("FAIL: %s:%d - Expected false, got true\n", __FILE__, __LINE__); \
            return false; \
        } \
    } while(0)

// Test function type
typedef bool (*test_function_t)(void);

// Test suite structure
typedef struct {
    const char* name;
    test_function_t* tests;
    int test_count;
} test_suite_t;

// Test framework functions
void run_test(const char* test_name, test_function_t test_func);
void run_test_suite(const test_suite_t* suite);
void print_test_summary(int passed, int total);

// Global test counters
extern int tests_passed;
extern int tests_failed;

#endif // TEST_FRAMEWORK_H


