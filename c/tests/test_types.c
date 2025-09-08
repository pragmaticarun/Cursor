#include "../include/test_framework.h"
#include <stdint.h>
#include <limits.h>
#include <float.h>

// Test functions for types
bool test_integer_types(void) {
    // Test basic integer types
    char c = 'A';
    int i = 42;
    long l = 123456789L;
    
    TEST_ASSERT(c == 'A');
    TEST_ASSERT(i == 42);
    TEST_ASSERT(l == 123456789L);
    
    // Test fixed-width types
    int8_t i8 = 127;
    int32_t i32 = 2147483647;
    
    TEST_ASSERT(i8 == 127);
    TEST_ASSERT(i32 == 2147483647);
    
    return true;
}

bool test_floating_types(void) {
    float f = 3.14f;
    double d = 3.141592653589793;
    
    TEST_ASSERT(f > 3.13f && f < 3.15f);
    TEST_ASSERT(d > 3.141592653589792 && d < 3.141592653589794);
    
    return true;
}

bool test_boolean_type(void) {
    bool b1 = true;
    bool b2 = false;
    bool b3 = 42;  // Non-zero should be true
    
    TEST_ASSERT_TRUE(b1);
    TEST_ASSERT_FALSE(b2);
    TEST_ASSERT_TRUE(b3);
    
    return true;
}

bool test_type_conversions(void) {
    int i = 42;
    float f = 3.14f;
    
    // Implicit conversion
    double result = i + f;
    TEST_ASSERT(result > 45.0 && result < 46.0);
    
    // Explicit conversion
    int converted = (int)f;
    TEST_ASSERT_EQUAL(3, converted);
    
    return true;
}

bool test_type_limits(void) {
    // Test that limits are reasonable
    TEST_ASSERT(INT_MAX > 0);
    TEST_ASSERT(INT_MIN < 0);
    TEST_ASSERT(UINT_MAX > INT_MAX);
    TEST_ASSERT(FLT_MAX > 0);
    TEST_ASSERT(FLT_MIN > 0);
    
    return true;
}

// Test suite
test_function_t type_tests[] = {
    test_integer_types,
    test_floating_types,
    test_boolean_type,
    test_type_conversions,
    test_type_limits
};

test_suite_t type_test_suite = {
    "Types",
    type_tests,
    sizeof(type_tests) / sizeof(type_tests[0])
};

int main(void) {
    printf("C Types Test Suite\n");
    printf("==================\n");
    
    run_test_suite(&type_test_suite);
    print_test_summary(tests_passed, tests_passed + tests_failed);
    
    return (tests_failed == 0) ? 0 : 1;
}


