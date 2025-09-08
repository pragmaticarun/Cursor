#include "test_framework.h"

// Global test counters
int tests_passed = 0;
int tests_failed = 0;

void run_test(const char* test_name, test_function_t test_func) {
    printf("Running test: %s... ", test_name);
    
    bool result = test_func();
    
    if (result) {
        printf("PASS\n");
        tests_passed++;
    } else {
        printf("FAIL\n");
        tests_failed++;
    }
}

void run_test_suite(const test_suite_t* suite) {
    printf("\n=== Running Test Suite: %s ===\n", suite->name);
    
    for (int i = 0; i < suite->test_count; i++) {
        char test_name[100];
        snprintf(test_name, sizeof(test_name), "test_%d", i + 1);
        run_test(test_name, suite->tests[i]);
    }
}

void print_test_summary(int passed, int total) {
    printf("\n=== Test Summary ===\n");
    printf("Total tests: %d\n", total);
    printf("Passed: %d\n", passed);
    printf("Failed: %d\n", total - passed);
    
    if (passed == total) {
        printf("All tests passed! ✓\n");
    } else {
        printf("Some tests failed! ✗\n");
    }
}

