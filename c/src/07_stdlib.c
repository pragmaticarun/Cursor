#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * C Standard Library Tutorial
 * Demonstrates memory management, utility functions, and system operations
 */

void demonstrate_memory_management(void) {
    printf("=== Memory Management ===\n");
    
    // malloc - allocate memory
    int* ptr1 = malloc(5 * sizeof(int));
    if (ptr1 == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    
    // Initialize allocated memory
    for (int i = 0; i < 5; i++) {
        ptr1[i] = (i + 1) * 10;
    }
    
    printf("malloc allocated array: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", ptr1[i]);
    }
    printf("\n");
    
    // calloc - allocate and initialize to zero
    int* ptr2 = calloc(3, sizeof(int));
    if (ptr2 == NULL) {
        printf("Memory allocation failed\n");
        free(ptr1);
        return;
    }
    
    printf("calloc allocated array (initialized to zero): ");
    for (int i = 0; i < 3; i++) {
        printf("%d ", ptr2[i]);
    }
    printf("\n");
    
    // realloc - resize memory
    int* ptr3 = realloc(ptr1, 8 * sizeof(int));
    if (ptr3 == NULL) {
        printf("Memory reallocation failed\n");
        free(ptr1);
        free(ptr2);
        return;
    }
    
    ptr1 = ptr3;  // Update pointer
    
    // Initialize new elements
    for (int i = 5; i < 8; i++) {
        ptr1[i] = (i + 1) * 10;
    }
    
    printf("realloc resized array: ");
    for (int i = 0; i < 8; i++) {
        printf("%d ", ptr1[i]);
    }
    printf("\n");
    
    // Free memory
    free(ptr1);
    free(ptr2);
    printf("Memory freed\n");
}

void demonstrate_string_conversion(void) {
    printf("\n=== String Conversion ===\n");
    
    // String to number conversion
    const char* str_num = "12345";
    const char* str_float = "3.14159";
    const char* str_hex = "FF";
    
    // strtol - string to long
    long num = strtol(str_num, NULL, 10);
    printf("strtol('%s', 10) = %ld\n", str_num, num);
    
    // strtol with different bases
    long hex_num = strtol(str_hex, NULL, 16);
    printf("strtol('%s', 16) = %ld\n", str_hex, hex_num);
    
    // strtod - string to double
    double float_num = strtod(str_float, NULL);
    printf("strtod('%s') = %f\n", str_float, float_num);
    
    // atoi, atol, atof - simple conversions
    int atoi_result = atoi("42");
    long atol_result = atol("123456789");
    double atof_result = atof("2.718");
    
    printf("atoi('42') = %d\n", atoi_result);
    printf("atol('123456789') = %ld\n", atol_result);
    printf("atof('2.718') = %f\n", atof_result);
}

// Comparison function for qsort
int compare_ints(const void* a, const void* b) {
    int ia = *(const int*)a;
    int ib = *(const int*)b;
    return (ia > ib) - (ia < ib);
}

void demonstrate_sorting_and_searching(void) {
    printf("\n=== Sorting and Searching ===\n");
    
    int numbers[] = {64, 34, 25, 12, 22, 11, 90};
    size_t size = sizeof(numbers) / sizeof(numbers[0]);
    
    printf("Original array: ");
    for (size_t i = 0; i < size; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    
    // qsort - quick sort
    qsort(numbers, size, sizeof(int), compare_ints);
    
    printf("Sorted array: ");
    for (size_t i = 0; i < size; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    
    // bsearch - binary search
    int key = 25;
    int* result = bsearch(&key, numbers, size, sizeof(int), compare_ints);
    if (result != NULL) {
        printf("Found %d at position %ld\n", key, result - numbers);
    } else {
        printf("%d not found\n", key);
    }
}


void demonstrate_random_numbers(void) {
    printf("\n=== Random Numbers ===\n");
    
    // Seed random number generator
    srand((unsigned int)time(NULL));
    
    printf("Random integers (0-99): ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", rand() % 100);
    }
    printf("\n");
    
    printf("Random floats (0.0-1.0): ");
    for (int i = 0; i < 5; i++) {
        printf("%.3f ", (double)rand() / RAND_MAX);
    }
    printf("\n");
    
    // Random number in range [min, max]
    int min = 10, max = 20;
    printf("Random numbers in range [%d, %d]: ", min, max);
    for (int i = 0; i < 5; i++) {
        int random_num = min + rand() % (max - min + 1);
        printf("%d ", random_num);
    }
    printf("\n");
}

void demonstrate_environment_variables(void) {
    printf("\n=== Environment Variables ===\n");
    
    // Get environment variable
    const char* home = getenv("HOME");
    if (home != NULL) {
        printf("HOME: %s\n", home);
    } else {
        printf("HOME not set\n");
    }
    
    const char* path = getenv("PATH");
    if (path != NULL) {
        printf("PATH: %s\n", path);
    } else {
        printf("PATH not set\n");
    }
    
    // Note: setenv is POSIX, not standard C
    // Environment variable setting is platform-specific
    printf("Environment variable operations:\n");
    printf("  getenv() - read environment variables\n");
    printf("  setenv() - set environment variables (POSIX)\n");
}

void demonstrate_system_operations(void) {
    printf("\n=== System Operations ===\n");
    
    // system - execute shell command
    printf("Executing 'echo Hello from system()':\n");
    int result = system("echo Hello from system()");
    printf("system() returned: %d\n", result);
    
    // exit - terminate program
    printf("exit() function terminates the program\n");
    printf("exit(0) - normal termination\n");
    printf("exit(1) - error termination\n");
    
    // abort - abnormal termination
    printf("abort() function causes abnormal termination\n");
}

void demonstrate_utility_functions(void) {
    printf("\n=== Utility Functions ===\n");
    
    // abs - absolute value
    int negative = -42;
    printf("abs(%d) = %d\n", negative, abs(negative));
    
    // labs - absolute value for long
    long long_negative = -123456789L;
    printf("labs(%ld) = %ld\n", long_negative, labs(long_negative));
    
    // div - division with remainder
    int dividend = 17, divisor = 5;
    div_t result = div(dividend, divisor);
    printf("div(%d, %d) = quotient: %d, remainder: %d\n", 
           dividend, divisor, result.quot, result.rem);
    
    // ldiv - division with remainder for long
    long long_dividend = 123456789L, long_divisor = 1000L;
    ldiv_t long_result = ldiv(long_dividend, long_divisor);
    printf("ldiv(%ld, %ld) = quotient: %ld, remainder: %ld\n", 
           long_dividend, long_divisor, long_result.quot, long_result.rem);
}

void demonstrate_memory_utilities(void) {
    printf("\n=== Memory Utilities ===\n");
    
    // Allocate memory for demonstration
    int* arr1 = malloc(5 * sizeof(int));
    int* arr2 = malloc(5 * sizeof(int));
    
    if (arr1 == NULL || arr2 == NULL) {
        printf("Memory allocation failed\n");
        free(arr1);
        free(arr2);
        return;
    }
    
    // Initialize arrays
    for (int i = 0; i < 5; i++) {
        arr1[i] = i + 1;
        arr2[i] = 0;
    }
    
    printf("arr1 before memcpy: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", arr1[i]);
    }
    printf("\n");
    
    printf("arr2 before memcpy: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", arr2[i]);
    }
    printf("\n");
    
    // memcpy - copy memory
    memcpy(arr2, arr1, 5 * sizeof(int));
    
    printf("arr2 after memcpy: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", arr2[i]);
    }
    printf("\n");
    
    // memset - set memory
    memset(arr1, 0, 5 * sizeof(int));
    
    printf("arr1 after memset(0): ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", arr1[i]);
    }
    printf("\n");
    
    // memcmp - compare memory
    int cmp_result = memcmp(arr1, arr2, 5 * sizeof(int));
    printf("memcmp(arr1, arr2): %d\n", cmp_result);
    
    free(arr1);
    free(arr2);
}

int main(void) {
    printf("C Standard Library Tutorial\n");
    printf("===========================\n");
    
    demonstrate_memory_management();
    demonstrate_string_conversion();
    demonstrate_sorting_and_searching();
    demonstrate_random_numbers();
    demonstrate_environment_variables();
    demonstrate_system_operations();
    demonstrate_utility_functions();
    demonstrate_memory_utilities();
    
    return 0;
}

