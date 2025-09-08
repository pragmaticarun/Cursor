#include "../include/test_framework.h"

// Test functions for control flow
bool test_conditional_statements(void) {
    int age = 25;
    bool has_license = true;
    
    // Test if/else logic
    bool is_adult = (age >= 18);
    TEST_ASSERT_TRUE(is_adult);
    
    bool can_drive = (age >= 18 && has_license);
    TEST_ASSERT_TRUE(can_drive);
    
    // Test ternary operator
    const char* status = (age >= 18) ? "adult" : "minor";
    TEST_ASSERT_EQUAL_STRING("adult", status);
    
    return true;
}

bool test_switch_statement(void) {
    int grade = 85;
    char letter_grade;
    
    switch (grade / 10) {
        case 10:
        case 9:
            letter_grade = 'A';
            break;
        case 8:
            letter_grade = 'B';
            break;
        case 7:
            letter_grade = 'C';
            break;
        case 6:
            letter_grade = 'D';
            break;
        default:
            letter_grade = 'F';
            break;
    }
    
    TEST_ASSERT_EQUAL('B', letter_grade);
    
    return true;
}

bool test_for_loop(void) {
    int sum = 0;
    
    // Test basic for loop
    for (int i = 1; i <= 5; i++) {
        sum += i;
    }
    
    TEST_ASSERT_EQUAL(15, sum);  // 1+2+3+4+5 = 15
    
    // Test nested loops
    int product = 1;
    for (int i = 1; i <= 3; i++) {
        for (int j = 1; j <= 2; j++) {
            product *= i * j;
        }
    }
    
    // Calculation: (1*1)*(1*2)*(2*1)*(2*2)*(3*1)*(3*2) = 1*2*2*4*3*6 = 288
    TEST_ASSERT_EQUAL(288, product);
    
    return true;
}

bool test_while_loop(void) {
    int count = 0;
    int target = 5;
    
    // Test while loop
    while (count < target) {
        count++;
    }
    
    TEST_ASSERT_EQUAL(target, count);
    
    // Test while loop with condition
    int number = 64;
    int power = 0;
    int temp = number;
    while (temp > 1) {
        temp /= 2;
        power++;
    }
    
    TEST_ASSERT_EQUAL(6, power);  // 64 = 2^6
    
    return true;
}

bool test_do_while_loop(void) {
    int value = 10;
    int count = 0;
    
    // Test do-while (executes at least once)
    do {
        value -= 2;
        count++;
    } while (value > 0);
    
    TEST_ASSERT_EQUAL(5, count);  // Should execute 5 times
    TEST_ASSERT_EQUAL(0, value);  // Final value should be 0
    
    return true;
}

bool test_break_continue(void) {
    int sum = 0;
    int break_count = 0;
    
    // Test break
    for (int i = 1; i <= 10; i++) {
        if (i == 6) {
            break;
        }
        sum += i;
        break_count++;
    }
    
    TEST_ASSERT_EQUAL(15, sum);  // 1+2+3+4+5 = 15
    TEST_ASSERT_EQUAL(5, break_count);
    
    // Test continue
    int odd_sum = 0;
    for (int i = 1; i <= 10; i++) {
        if (i % 2 == 0) {
            continue;  // Skip even numbers
        }
        odd_sum += i;
    }
    
    TEST_ASSERT_EQUAL(25, odd_sum);  // 1+3+5+7+9 = 25
    
    return true;
}

// Test suite
test_function_t control_flow_tests[] = {
    test_conditional_statements,
    test_switch_statement,
    test_for_loop,
    test_while_loop,
    test_do_while_loop,
    test_break_continue
};

test_suite_t control_flow_test_suite = {
    "Control Flow",
    control_flow_tests,
    sizeof(control_flow_tests) / sizeof(control_flow_tests[0])
};

int main(void) {
    printf("C Control Flow Test Suite\n");
    printf("=========================\n");
    
    run_test_suite(&control_flow_test_suite);
    print_test_summary(tests_passed, tests_passed + tests_failed);
    
    return (tests_failed == 0) ? 0 : 1;
}

