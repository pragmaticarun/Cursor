#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/**
 * C Functions Tutorial
 * Demonstrates function definitions, prototypes, parameters, and advanced features
 */

// Function prototypes
int add(int a, int b);
int multiply(int a, int b);
void print_array(int arr[], size_t size);
int factorial(int n);
void swap(int* a, int* b);
int* create_array(size_t size);
void demonstrate_static_variable(void);
void demonstrate_recursion(int n);
int sum_variadic(int count, ...);
void demonstrate_function_pointers(void);

// Function definitions
int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

void print_array(int arr[], size_t size) {
    printf("Array: [");
    for (size_t i = 0; i < size; i++) {
        printf("%d", arr[i]);
        if (i < size - 1) printf(", ");
    }
    printf("]\n");
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

void demonstrate_static_variable(void) {
    static int call_count = 0;
    call_count++;
    printf("Function called %d time(s)\n", call_count);
}

void demonstrate_recursion(int n) {
    if (n <= 0) {
        printf("\n");
        return;
    }
    
    printf("%d ", n);
    demonstrate_recursion(n - 1);
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

void demonstrate_function_pointers(void) {
    printf("\n=== Function Pointers ===\n");
    
    // Function pointer declaration
    int (*operation)(int, int);
    
    // Assign function to pointer
    operation = add;
    printf("5 + 3 = %d\n", operation(5, 3));
    
    operation = multiply;
    printf("5 * 3 = %d\n", operation(5, 3));
    
    // Array of function pointers
    int (*operations[])(int, int) = {add, multiply};
    printf("Using array: add(4, 2) = %d, multiply(4, 2) = %d\n", 
           operations[0](4, 2), operations[1](4, 2));
}

void demonstrate_parameter_passing(void) {
    printf("\n=== Parameter Passing ===\n");
    
    // Pass by value
    int a = 10, b = 20;
    printf("Before swap: a=%d, b=%d\n", a, b);
    swap(&a, &b);
    printf("After swap: a=%d, b=%d\n", a, b);
    
    // Array passing (arrays decay to pointers)
    int numbers[] = {1, 2, 3, 4, 5};
    size_t size = sizeof(numbers) / sizeof(numbers[0]);
    print_array(numbers, size);
}

void demonstrate_memory_management(void) {
    printf("\n=== Memory Management ===\n");
    
    // Dynamic memory allocation
    size_t size = 5;
    int* dynamic_array = create_array(size);
    
    if (dynamic_array != NULL) {
        printf("Dynamic array created: ");
        print_array(dynamic_array, size);
        free(dynamic_array);
        printf("Memory freed\n");
    } else {
        printf("Failed to allocate memory\n");
    }
}

void demonstrate_scope_and_lifetime(void) {
    printf("\n=== Scope and Lifetime ===\n");
    
    // Local variables
    int local_var = 42;
    printf("Local variable: %d\n", local_var);
    
    // Static local variable
    demonstrate_static_variable();
    demonstrate_static_variable();
    demonstrate_static_variable();
    
    // Block scope
    {
        int block_var = 100;
        printf("Block variable: %d\n", block_var);
    }
    // block_var is not accessible here
}

void demonstrate_recursive_functions(void) {
    printf("\n=== Recursive Functions ===\n");
    
    int n = 5;
    printf("Factorial of %d: %d\n", n, factorial(n));
    
    printf("Recursive countdown from %d: ", n);
    demonstrate_recursion(n);
}

void demonstrate_variadic_functions(void) {
    printf("\n=== Variadic Functions ===\n");
    
    printf("Sum of 3 numbers (1, 2, 3): %d\n", sum_variadic(3, 1, 2, 3));
    printf("Sum of 5 numbers (1, 2, 3, 4, 5): %d\n", sum_variadic(5, 1, 2, 3, 4, 5));
}

// Inline function example
static inline int square(int x) {
    return x * x;
}

void demonstrate_inline_functions(void) {
    printf("\n=== Inline Functions ===\n");
    
    int x = 5;
    printf("Square of %d: %d\n", x, square(x));
}

int main(void) {
    printf("C Functions Tutorial\n");
    printf("====================\n");
    
    demonstrate_parameter_passing();
    demonstrate_memory_management();
    demonstrate_scope_and_lifetime();
    demonstrate_recursive_functions();
    demonstrate_variadic_functions();
    demonstrate_inline_functions();
    demonstrate_function_pointers();
    
    return 0;
}


