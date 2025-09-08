#include "../include/test_framework.h"
#include <stdio.h>
#include <stdlib.h>

// External test functions from other test files
extern test_suite_t type_test_suite;
extern test_suite_t control_flow_test_suite;
extern test_suite_t string_test_suite;
extern test_suite_t function_test_suite;
extern test_suite_t pointer_test_suite;
extern test_suite_t stdlib_test_suite;

int main(void) {
    printf("C Programming Tutorial - Complete Test Suite\n");
    printf("============================================\n\n");
    
    int total_passed = 0;
    int total_failed = 0;
    
    // Reset global counters
    tests_passed = 0;
    tests_failed = 0;
    
    // Run all test suites
    run_test_suite(&type_test_suite);
    total_passed += tests_passed;
    total_failed += tests_failed;
    
    tests_passed = 0;
    tests_failed = 0;
    run_test_suite(&control_flow_test_suite);
    total_passed += tests_passed;
    total_failed += tests_failed;
    
    tests_passed = 0;
    tests_failed = 0;
    run_test_suite(&string_test_suite);
    total_passed += tests_passed;
    total_failed += tests_failed;
    
    tests_passed = 0;
    tests_failed = 0;
    run_test_suite(&function_test_suite);
    total_passed += tests_passed;
    total_failed += tests_failed;
    
    tests_passed = 0;
    tests_failed = 0;
    run_test_suite(&pointer_test_suite);
    total_passed += tests_passed;
    total_failed += tests_failed;
    
    tests_passed = 0;
    tests_failed = 0;
    run_test_suite(&stdlib_test_suite);
    total_passed += tests_passed;
    total_failed += tests_failed;
    
    // Print overall summary
    printf("\n=== OVERALL TEST SUMMARY ===\n");
    printf("Total tests run: %d\n", total_passed + total_failed);
    printf("Passed: %d\n", total_passed);
    printf("Failed: %d\n", total_failed);
    
    if (total_failed == 0) {
        printf("\n🎉 ALL TESTS PASSED! 🎉\n");
        printf("The C programming tutorial is working correctly.\n");
    } else {
        printf("\n❌ Some tests failed. Please check the output above.\n");
    }
    
    return (total_failed == 0) ? 0 : 1;
}