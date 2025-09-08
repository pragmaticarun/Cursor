#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * C Arrays and Strings Tutorial
 * Demonstrates array operations, string handling, and memory management
 */

void demonstrate_arrays(void) {
    printf("=== Arrays ===\n");
    
    // Array declaration and initialization
    int numbers[5] = {1, 2, 3, 4, 5};
    int matrix[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    
    // Array access and modification
    printf("Original array: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    
    // Modify array elements
    numbers[0] = 10;
    numbers[4] = 50;
    
    printf("Modified array: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    
    // 2D array
    printf("2D array (3x3 matrix):\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    
    // Array size calculation
    size_t array_size = sizeof(numbers) / sizeof(numbers[0]);
    printf("Array size: %zu elements\n", array_size);
}

void demonstrate_strings(void) {
    printf("\n=== Strings ===\n");
    
    // String literals
    const char* str1 = "Hello, World!";
    char str2[] = "C Programming";
    char str3[20] = "Dynamic";
    
    printf("String 1: %s\n", str1);
    printf("String 2: %s\n", str2);
    printf("String 3: %s\n", str3);
    
    // String length
    printf("Length of str1: %zu\n", strlen(str1));
    printf("Length of str2: %zu\n", strlen(str2));
    
    // String concatenation
    strcat(str3, " String");
    printf("After concatenation: %s\n", str3);
    
    // String comparison
    int cmp_result = strcmp(str1, str2);
    if (cmp_result == 0) {
        printf("Strings are equal\n");
    } else if (cmp_result < 0) {
        printf("str1 comes before str2\n");
    } else {
        printf("str1 comes after str2\n");
    }
    
    // String copying
    char copy[50];
    strcpy(copy, str1);
    printf("Copied string: %s\n", copy);
    
    // Safe string operations
    char safe_copy[20];
    strncpy(safe_copy, str1, sizeof(safe_copy) - 1);
    safe_copy[sizeof(safe_copy) - 1] = '\0';  // Ensure null termination
    printf("Safe copy: %s\n", safe_copy);
}

void demonstrate_dynamic_arrays(void) {
    printf("\n=== Dynamic Arrays ===\n");
    
    // Allocate dynamic array
    size_t size = 5;
    int* dynamic_array = malloc(size * sizeof(int));
    
    if (dynamic_array == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    
    // Initialize dynamic array
    for (size_t i = 0; i < size; i++) {
        dynamic_array[i] = (int)(i + 1) * 10;
    }
    
    printf("Dynamic array: ");
    for (size_t i = 0; i < size; i++) {
        printf("%d ", dynamic_array[i]);
    }
    printf("\n");
    
    // Resize array
    size_t new_size = 8;
    int* resized_array = realloc(dynamic_array, new_size * sizeof(int));
    
    if (resized_array == NULL) {
        printf("Memory reallocation failed\n");
        free(dynamic_array);
        return;
    }
    
    dynamic_array = resized_array;
    
    // Initialize new elements
    for (size_t i = size; i < new_size; i++) {
        dynamic_array[i] = (int)(i + 1) * 10;
    }
    
    printf("Resized array: ");
    for (size_t i = 0; i < new_size; i++) {
        printf("%d ", dynamic_array[i]);
    }
    printf("\n");
    
    free(dynamic_array);
}

void demonstrate_string_functions(void) {
    printf("\n=== String Functions ===\n");
    
    char text[] = "Hello, World!";
    char buffer[100];
    
    // String length
    printf("Length of '%s': %zu\n", text, strlen(text));
    
    // String copy
    strcpy(buffer, text);
    printf("Copied: %s\n", buffer);
    
    // String concatenation
    strcat(buffer, " Welcome to C!");
    printf("After concatenation: %s\n", buffer);
    
    // String comparison
    char str1[] = "apple";
    char str2[] = "banana";
    char str3[] = "apple";
    
    printf("strcmp('%s', '%s'): %d\n", str1, str2, strcmp(str1, str2));
    printf("strcmp('%s', '%s'): %d\n", str1, str3, strcmp(str1, str3));
    
    // Case-insensitive comparison (POSIX function, may not be available)
    char str4[] = "APPLE";
    // Note: strcasecmp is POSIX, not standard C
    // printf("strcasecmp('%s', '%s'): %d\n", str1, str4, strcasecmp(str1, str4));
    printf("Case comparison: '%s' vs '%s'\n", str1, str4);
    
    // String search
    char* found = strstr(text, "World");
    if (found) {
        printf("Found 'World' at position: %ld\n", found - text);
    }
    
    // Character search
    char* char_found = strchr(text, 'o');
    if (char_found) {
        printf("Found 'o' at position: %ld\n", char_found - text);
    }
}

void demonstrate_character_arrays(void) {
    printf("\n=== Character Arrays ===\n");
    
    // Character array initialization
    char word1[] = {'H', 'e', 'l', 'l', 'o', '\0'};
    char word2[] = "World";
    
    printf("Word 1: %s\n", word1);
    printf("Word 2: %s\n", word2);
    
    // Character manipulation
    char text[] = "Hello, World!";
    printf("Original: %s\n", text);
    
    // Convert to uppercase
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] >= 'a' && text[i] <= 'z') {
            text[i] = text[i] - 'a' + 'A';
        }
    }
    printf("Uppercase: %s\n", text);
    
    // Convert back to lowercase
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] >= 'A' && text[i] <= 'Z') {
            text[i] = text[i] - 'A' + 'a';
        }
    }
    printf("Lowercase: %s\n", text);
}

void demonstrate_array_operations(void) {
    printf("\n=== Array Operations ===\n");
    
    int arr[] = {5, 2, 8, 1, 9, 3};
    size_t size = sizeof(arr) / sizeof(arr[0]);
    
    printf("Original array: ");
    for (size_t i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    // Find maximum and minimum
    int max = arr[0], min = arr[0];
    for (size_t i = 1; i < size; i++) {
        if (arr[i] > max) max = arr[i];
        if (arr[i] < min) min = arr[i];
    }
    printf("Maximum: %d, Minimum: %d\n", max, min);
    
    // Calculate sum and average
    int sum = 0;
    for (size_t i = 0; i < size; i++) {
        sum += arr[i];
    }
    double average = (double)sum / size;
    printf("Sum: %d, Average: %.2f\n", sum, average);
    
    // Reverse array
    for (size_t i = 0; i < size / 2; i++) {
        int temp = arr[i];
        arr[i] = arr[size - 1 - i];
        arr[size - 1 - i] = temp;
    }
    
    printf("Reversed array: ");
    for (size_t i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main(void) {
    printf("C Arrays and Strings Tutorial\n");
    printf("=============================\n");
    
    demonstrate_arrays();
    demonstrate_strings();
    demonstrate_dynamic_arrays();
    demonstrate_string_functions();
    demonstrate_character_arrays();
    demonstrate_array_operations();
    
    return 0;
}

