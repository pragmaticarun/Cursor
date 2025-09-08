#include "../include/test_framework.h"
#include <stdlib.h>
#include <stdarg.h>

// Test functions for function features
bool test_function_basics(void) {
    // Test simple function call
    int result = add(5, 3);
    TEST_ASSERT_EQUAL(8, result);
    
    result = multiply(4, 6);
    TEST_ASSERT_EQUAL(24, result);
    
    return true;
}

bool test_recursive_functions(void) {
    // Test factorial function
    TEST_ASSERT_EQUAL(1, factorial(0));
    TEST_ASSERT_EQUAL(1, factorial(1));
    TEST_ASSERT_EQUAL(2, factorial(2));
    TEST_ASSERT_EQUAL(6, factorial(3));
    TEST_ASSERT_EQUAL(24, factorial(4));
    TEST_ASSERT_EQUAL(120, factorial(5));
    
    return true;
}

bool test_parameter_passing(void) {
    int a = 10, b = 20;
    
    // Test pass by reference
    swap(&a, &b);
    TEST_ASSERT_EQUAL(20, a);
    TEST_ASSERT_EQUAL(10, b);
    
    return true;
}

bool test_dynamic_memory(void) {
    // Test dynamic array creation
    int* arr = create_array(5);
    TEST_ASSERT_NOT_NULL(arr);
    
    if (arr != NULL) {
        // Check array contents
        for (int i = 0; i < 5; i++) {
            TEST_ASSERT_EQUAL(i + 1, arr[i]);
        }
        free(arr);
    }
    
    return true;
}

bool test_variadic_functions(void) {
    // Test variadic function
    int sum1 = sum_variadic(3, 1, 2, 3);
    TEST_ASSERT_EQUAL(6, sum1);
    
    int sum2 = sum_variadic(5, 10, 20, 30, 40, 50);
    TEST_ASSERT_EQUAL(150, sum2);
    
    return true;
}

// Helper functions for testing
int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int* create_array(size_t size) {
    int* arr = malloc(size * sizeof(int));
    if (arr == NULL) {
        return NULL;
    }
    
    for (size_t i = 0; i < size; i++) {
        arr[i] = (int)(i + 1);
    }
    
    return arr;
}

int sum_variadic(int count, ...) {
    va_list args;
    va_start(args, count);
    
    int sum = 0;
    for (int i = 0; i < count; i++) {
        sum += va_arg(args, int);
    }
    
    va_end(args);
    return sum;
}

// Test suite
test_function_t function_tests[] = {
    test_function_basics,
    test_recursive_functions,
    test_parameter_passing,
    test_dynamic_memory,
    test_variadic_functions
};

test_suite_t function_test_suite = {
    "Functions",
    function_tests,
    sizeof(function_tests) / sizeof(function_tests[0])
};

int main(void) {
    printf("C Functions Test Suite\n");
    printf("======================\n");
    
    run_test_suite(&function_test_suite);
    print_test_summary(tests_passed, tests_passed + tests_failed);
    
    return (tests_failed == 0) ? 0 : 1;
}


