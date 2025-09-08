#include "../include/test_framework.h"
#include <stdlib.h>
#include <string.h>

// Test functions for pointer operations
bool test_basic_pointers(void) {
    int value = 42;
    int* ptr = &value;
    
    // Test pointer assignment and dereferencing
    TEST_ASSERT_EQUAL(42, *ptr);
    TEST_ASSERT_EQUAL((intptr_t)&value, (intptr_t)ptr);
    
    // Test modification through pointer
    *ptr = 100;
    TEST_ASSERT_EQUAL(100, value);
    
    return true;
}

bool test_pointer_arithmetic(void) {
    int arr[] = {10, 20, 30, 40, 50};
    int* ptr = arr;
    
    // Test pointer arithmetic
    TEST_ASSERT_EQUAL(10, *ptr);
    TEST_ASSERT_EQUAL(20, *(ptr + 1));
    TEST_ASSERT_EQUAL(30, *(ptr + 2));
    
    // Test array indexing through pointer
    TEST_ASSERT_EQUAL(10, ptr[0]);
    TEST_ASSERT_EQUAL(20, ptr[1]);
    TEST_ASSERT_EQUAL(30, ptr[2]);
    
    return true;
}

bool test_dynamic_memory_pointers(void) {
    // Test malloc
    int* ptr = malloc(sizeof(int));
    TEST_ASSERT_NOT_NULL(ptr);
    
    if (ptr != NULL) {
        *ptr = 42;
        TEST_ASSERT_EQUAL(42, *ptr);
        free(ptr);
    }
    
    // Test calloc
    int* arr = calloc(5, sizeof(int));
    TEST_ASSERT_NOT_NULL(arr);
    
    if (arr != NULL) {
        // Check that calloc initializes to zero
        for (int i = 0; i < 5; i++) {
            TEST_ASSERT_EQUAL(0, arr[i]);
        }
        free(arr);
    }
    
    return true;
}

bool test_pointer_to_pointer(void) {
    int value = 42;
    int* ptr1 = &value;
    int** ptr2 = &ptr1;
    
    // Test double pointer
    TEST_ASSERT_EQUAL(42, **ptr2);
    TEST_ASSERT_EQUAL((intptr_t)ptr1, (intptr_t)*ptr2);
    
    // Test modification through double pointer
    **ptr2 = 100;
    TEST_ASSERT_EQUAL(100, value);
    
    return true;
}

bool test_function_pointers(void) {
    // Test function pointer
    int (*operation)(int, int);
    
    int add(int a, int b) { return a + b; }
    int multiply(int a, int b) { return a * b; }
    
    operation = add;
    TEST_ASSERT_EQUAL(8, operation(5, 3));
    
    operation = multiply;
    TEST_ASSERT_EQUAL(15, operation(5, 3));
    
    return true;
}

bool test_string_pointers(void) {
    const char* str = "Hello, World!";
    
    // Test string pointer
    TEST_ASSERT_EQUAL('H', *str);
    TEST_ASSERT_EQUAL('e', *(str + 1));
    TEST_ASSERT_EQUAL('!', *(str + 12));
    
    // Test string length through pointer
    int len = 0;
    const char* p = str;
    while (*p != '\0') {
        len++;
        p++;
    }
    TEST_ASSERT_EQUAL(13, len);
    
    return true;
}

bool test_void_pointers(void) {
    int int_value = 42;
    float float_value = 3.14f;
    char char_value = 'A';
    
    void* void_ptr;
    
    // Test void pointer with different types
    void_ptr = &int_value;
    TEST_ASSERT_EQUAL(42, *(int*)void_ptr);
    
    void_ptr = &float_value;
    TEST_ASSERT_TRUE(*(float*)void_ptr > 3.13f && *(float*)void_ptr < 3.15f);
    
    void_ptr = &char_value;
    TEST_ASSERT_EQUAL('A', *(char*)void_ptr);
    
    return true;
}

bool test_const_pointers(void) {
    int value1 = 10;
    int value2 = 20;
    
    // Test pointer to const
    const int* ptr1 = &value1;
    TEST_ASSERT_EQUAL(10, *ptr1);
    ptr1 = &value2;  // Can change what pointer points to
    TEST_ASSERT_EQUAL(20, *ptr1);
    
    // Test const pointer
    int* const ptr2 = &value1;
    TEST_ASSERT_EQUAL(10, *ptr2);
    *ptr2 = 30;  // Can modify value
    TEST_ASSERT_EQUAL(30, value1);
    
    return true;
}

// Test suite
test_function_t pointer_tests[] = {
    test_basic_pointers,
    test_pointer_arithmetic,
    test_dynamic_memory_pointers,
    test_pointer_to_pointer,
    test_function_pointers,
    test_string_pointers,
    test_void_pointers,
    test_const_pointers
};

test_suite_t pointer_test_suite = {
    "Pointers",
    pointer_tests,
    sizeof(pointer_tests) / sizeof(pointer_tests[0])
};

int main(void) {
    printf("C Pointers Test Suite\n");
    printf("=====================\n");
    
    run_test_suite(&pointer_test_suite);
    print_test_summary(tests_passed, tests_passed + tests_failed);
    
    return (tests_failed == 0) ? 0 : 1;
}


