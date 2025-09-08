#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * C Control Flow Tutorial
 * Demonstrates if/else, switch, loops, and control statements
 */

void demonstrate_conditional_statements(void) {
    printf("=== Conditional Statements ===\n");
    
    int age = 25;
    bool has_license = true;
    
    // Basic if/else
    if (age >= 18) {
        printf("Person is an adult\n");
    } else {
        printf("Person is a minor\n");
    }
    
    // Multiple conditions
    if (age >= 18 && has_license) {
        printf("Can drive a car\n");
    } else if (age >= 16 && has_license) {
        printf("Can drive with restrictions\n");
    } else {
        printf("Cannot drive\n");
    }
    
    // Ternary operator
    const char* status = (age >= 18) ? "adult" : "minor";
    printf("Status: %s\n", status);
    
    // Nested conditions
    if (age >= 21) {
        printf("Can vote and drink\n");
    } else if (age >= 18) {
        printf("Can vote but cannot drink\n");
    } else {
        printf("Cannot vote or drink\n");
    }
}

void demonstrate_switch_statement(void) {
    printf("\n=== Switch Statement ===\n");
    
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
    
    printf("Grade %d = %c\n", grade, letter_grade);
    
    // Switch with enum
    enum day { MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY };
    enum day today = WEDNESDAY;
    
    switch (today) {
        case MONDAY:
        case TUESDAY:
        case WEDNESDAY:
        case THURSDAY:
        case FRIDAY:
            printf("It's a weekday\n");
            break;
        case SATURDAY:
        case SUNDAY:
            printf("It's a weekend\n");
            break;
    }
}

void demonstrate_for_loop(void) {
    printf("\n=== For Loop ===\n");
    
    // Basic for loop
    printf("Counting 1 to 5: ");
    for (int i = 1; i <= 5; i++) {
        printf("%d ", i);
    }
    printf("\n");
    
    // For loop with step
    printf("Even numbers 0 to 10: ");
    for (int i = 0; i <= 10; i += 2) {
        printf("%d ", i);
    }
    printf("\n");
    
    // Reverse counting
    printf("Countdown 5 to 1: ");
    for (int i = 5; i >= 1; i--) {
        printf("%d ", i);
    }
    printf("\n");
    
    // Nested loops
    printf("Multiplication table (3x3):\n");
    for (int i = 1; i <= 3; i++) {
        for (int j = 1; j <= 3; j++) {
            printf("%d x %d = %d\t", i, j, i * j);
        }
        printf("\n");
    }
}

void demonstrate_while_loop(void) {
    printf("\n=== While Loop ===\n");
    
    // Basic while loop
    int count = 1;
    printf("While loop counting 1 to 3: ");
    while (count <= 3) {
        printf("%d ", count);
        count++;
    }
    printf("\n");
    
    // While loop with condition
    int number = 64;
    int power = 0;
    int temp = number;
    while (temp > 1) {
        temp /= 2;
        power++;
    }
    printf("%d is 2^%d\n", number, power);
}

void demonstrate_do_while_loop(void) {
    printf("\n=== Do-While Loop ===\n");
    
    // Do-while executes at least once
    int value = 10;
    printf("Do-while with value %d: ", value);
    do {
        printf("%d ", value);
        value -= 2;
    } while (value > 0);
    printf("\n");
    
    // Do-while for input validation (simulated)
    int input = 0;
    do {
        input++;  // Simulate getting valid input
    } while (input < 5);
    printf("Got valid input: %d\n", input);
}

void demonstrate_break_continue(void) {
    printf("\n=== Break and Continue ===\n");
    
    // Break example - find first even number
    printf("Finding first even number in 1-10: ");
    for (int i = 1; i <= 10; i++) {
        if (i % 2 == 0) {
            printf("%d (found!)\n", i);
            break;
        }
        printf("%d ", i);
    }
    
    // Continue example - print odd numbers only
    printf("Odd numbers 1-10: ");
    for (int i = 1; i <= 10; i++) {
        if (i % 2 == 0) {
            continue;  // Skip even numbers
        }
        printf("%d ", i);
    }
    printf("\n");
    
    // Break in nested loops
    printf("Breaking from nested loop:\n");
    for (int i = 1; i <= 3; i++) {
        for (int j = 1; j <= 3; j++) {
            if (i == 2 && j == 2) {
                printf("Breaking at i=%d, j=%d\n", i, j);
                goto outer_break;  // Break from outer loop
            }
            printf("i=%d, j=%d\n", i, j);
        }
    }
    outer_break:;
}

void demonstrate_goto_statement(void) {
    printf("\n=== Goto Statement ===\n");
    
    // Goto for error handling
    int* ptr = NULL;
    
    // Simulate allocation
    ptr = malloc(sizeof(int));
    if (ptr == NULL) {
        printf("Memory allocation failed\n");
        goto cleanup;
    }
    
    *ptr = 42;
    printf("Allocated memory, value: %d\n", *ptr);
    
cleanup:
    if (ptr != NULL) {
        free(ptr);
        printf("Memory freed\n");
    }
}

int main(void) {
    printf("C Control Flow Tutorial\n");
    printf("=======================\n");
    
    demonstrate_conditional_statements();
    demonstrate_switch_statement();
    demonstrate_for_loop();
    demonstrate_while_loop();
    demonstrate_do_while_loop();
    demonstrate_break_continue();
    demonstrate_goto_statement();
    
    return 0;
}

