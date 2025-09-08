#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * C Pointers Tutorial
 * Demonstrates pointer operations, memory management, and advanced pointer concepts
 */

void demonstrate_basic_pointers(void) {
    printf("=== Basic Pointers ===\n");
    
    int value = 42;
    int* ptr = &value;  // Pointer to value
    
    printf("Value: %d\n", value);
    printf("Address of value: %p\n", (void*)&value);
    printf("Pointer ptr: %p\n", (void*)ptr);
    printf("Value through pointer: %d\n", *ptr);
    
    // Modify value through pointer
    *ptr = 100;
    printf("After modification through pointer: %d\n", value);
    
    // Pointer arithmetic
    int arr[] = {10, 20, 30, 40, 50};
    int* arr_ptr = arr;  // Points to first element
    
    printf("\nArray through pointer arithmetic:\n");
    for (int i = 0; i < 5; i++) {
        printf("arr[%d] = %d (address: %p)\n", i, *(arr_ptr + i), (void*)(arr_ptr + i));
    }
}

void demonstrate_pointer_arithmetic(void) {
    printf("\n=== Pointer Arithmetic ===\n");
    
    int numbers[] = {1, 2, 3, 4, 5};
    int* ptr = numbers;
    
    printf("Original array: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    
    // Different ways to access array elements
    printf("Accessing elements:\n");
    printf("ptr[0] = %d\n", ptr[0]);
    printf("*(ptr + 1) = %d\n", *(ptr + 1));
    printf("ptr[2] = %d\n", ptr[2]);
    
    // Increment pointer
    ptr++;
    printf("After ptr++: *ptr = %d\n", *ptr);
    
    // Decrement pointer
    ptr--;
    printf("After ptr--: *ptr = %d\n", *ptr);
    
    // Pointer difference
    int* ptr2 = &numbers[4];
    printf("Difference between pointers: %ld\n", ptr2 - ptr);
}

void demonstrate_dynamic_memory(void) {
    printf("\n=== Dynamic Memory Management ===\n");
    
    // Allocate memory for single integer
    int* single_int = malloc(sizeof(int));
    if (single_int == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    
    *single_int = 42;
    printf("Single integer: %d\n", *single_int);
    free(single_int);
    
    // Allocate memory for array
    size_t size = 5;
    int* dynamic_array = malloc(size * sizeof(int));
    if (dynamic_array == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    
    // Initialize array
    for (size_t i = 0; i < size; i++) {
        dynamic_array[i] = (int)(i + 1) * 10;
    }
    
    printf("Dynamic array: ");
    for (size_t i = 0; i < size; i++) {
        printf("%d ", dynamic_array[i]);
    }
    printf("\n");
    
    // Reallocate memory
    size_t new_size = 8;
    int* resized = realloc(dynamic_array, new_size * sizeof(int));
    if (resized == NULL) {
        printf("Memory reallocation failed\n");
        free(dynamic_array);
        return;
    }
    
    dynamic_array = resized;
    
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

void demonstrate_pointer_to_pointer(void) {
    printf("\n=== Pointer to Pointer ===\n");
    
    int value = 42;
    int* ptr1 = &value;
    int** ptr2 = &ptr1;
    
    printf("Value: %d\n", value);
    printf("Pointer to value: %p\n", (void*)ptr1);
    printf("Pointer to pointer: %p\n", (void*)ptr2);
    printf("Value through double pointer: %d\n", **ptr2);
    
    // Modify through double pointer
    **ptr2 = 100;
    printf("After modification through double pointer: %d\n", value);
}

void demonstrate_function_pointers(void) {
    printf("\n=== Function Pointers ===\n");
    
    // Function pointer declaration
    int (*operation)(int, int);
    
    // Simple functions
    int add(int a, int b) { return a + b; }
    int multiply(int a, int b) { return a * b; }
    int subtract(int a, int b) { return a - b; }
    
    // Assign functions to pointer
    operation = add;
    printf("5 + 3 = %d\n", operation(5, 3));
    
    operation = multiply;
    printf("5 * 3 = %d\n", operation(5, 3));
    
    operation = subtract;
    printf("5 - 3 = %d\n", operation(5, 3));
    
    // Array of function pointers
    int (*operations[])(int, int) = {add, subtract, multiply};
    const char* names[] = {"add", "subtract", "multiply"};
    
    printf("\nUsing array of function pointers:\n");
    for (int i = 0; i < 3; i++) {
        printf("5 %s 3 = %d\n", names[i], operations[i](5, 3));
    }
}

void demonstrate_string_pointers(void) {
    printf("\n=== String Pointers ===\n");
    
    // String literals
    const char* str1 = "Hello";
    const char* str2 = "World";
    
    printf("String 1: %s\n", str1);
    printf("String 2: %s\n", str2);
    
    // Array of string pointers
    const char* words[] = {"apple", "banana", "cherry", "date"};
    size_t word_count = sizeof(words) / sizeof(words[0]);
    
    printf("Array of strings:\n");
    for (size_t i = 0; i < word_count; i++) {
        printf("words[%zu] = %s\n", i, words[i]);
    }
    
    // Dynamic string allocation
    char* dynamic_string = malloc(20);
    if (dynamic_string != NULL) {
        strcpy(dynamic_string, "Dynamic String");
        printf("Dynamic string: %s\n", dynamic_string);
        free(dynamic_string);
    }
}

void demonstrate_void_pointers(void) {
    printf("\n=== Void Pointers ===\n");
    
    int int_value = 42;
    float float_value = 3.14f;
    char char_value = 'A';
    
    void* void_ptr;
    
    // Point to different types
    void_ptr = &int_value;
    printf("Integer through void pointer: %d\n", *(int*)void_ptr);
    
    void_ptr = &float_value;
    printf("Float through void pointer: %.2f\n", *(float*)void_ptr);
    
    void_ptr = &char_value;
    printf("Char through void pointer: %c\n", *(char*)void_ptr);
    
    // Generic function using void pointer
    void print_value(void* ptr, char type) {
        switch (type) {
            case 'i':
                printf("Integer: %d\n", *(int*)ptr);
                break;
            case 'f':
                printf("Float: %.2f\n", *(float*)ptr);
                break;
            case 'c':
                printf("Char: %c\n", *(char*)ptr);
                break;
        }
    }
    
    printf("\nGeneric function:\n");
    print_value(&int_value, 'i');
    print_value(&float_value, 'f');
    print_value(&char_value, 'c');
}

void demonstrate_const_pointers(void) {
    printf("\n=== Const Pointers ===\n");
    
    int value1 = 10;
    int value2 = 20;
    
    // Pointer to const
    const int* ptr1 = &value1;
    printf("Value through const pointer: %d\n", *ptr1);
    // *ptr1 = 30;  // Error: cannot modify through const pointer
    ptr1 = &value2;  // OK: can change what pointer points to
    printf("After changing pointer: %d\n", *ptr1);
    
    // Const pointer
    int* const ptr2 = &value1;
    printf("Value through const pointer: %d\n", *ptr2);
    *ptr2 = 30;  // OK: can modify value
    printf("After modification: %d\n", *ptr2);
    // ptr2 = &value2;  // Error: cannot change what pointer points to
    
    // Const pointer to const
    const int* const ptr3 = &value1;
    printf("Value through const pointer to const: %d\n", *ptr3);
    // *ptr3 = 40;  // Error: cannot modify
    // ptr3 = &value2;  // Error: cannot change pointer
}

void demonstrate_pointer_arrays(void) {
    printf("\n=== Pointer Arrays ===\n");
    
    int a = 1, b = 2, c = 3, d = 4;
    int* ptr_array[] = {&a, &b, &c, &d};
    
    printf("Array of pointers:\n");
    for (int i = 0; i < 4; i++) {
        printf("ptr_array[%d] points to: %d\n", i, *ptr_array[i]);
    }
    
    // 2D array using pointers
    int matrix[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int* row_pointers[3] = {matrix[0], matrix[1], matrix[2]};
    
    printf("\n2D array through pointer array:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d ", row_pointers[i][j]);
        }
        printf("\n");
    }
}

int main(void) {
    printf("C Pointers Tutorial\n");
    printf("===================\n");
    
    demonstrate_basic_pointers();
    demonstrate_pointer_arithmetic();
    demonstrate_dynamic_memory();
    demonstrate_pointer_to_pointer();
    demonstrate_function_pointers();
    demonstrate_string_pointers();
    demonstrate_void_pointers();
    demonstrate_const_pointers();
    demonstrate_pointer_arrays();
    
    return 0;
}

