#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * C Best Practices Tutorial
 * Demonstrates coding standards, error handling, and professional practices
 */

// Good: Use const for read-only data
const char* ERROR_MESSAGES[] = {
    "Success",
    "Invalid argument",
    "Memory allocation failed",
    "File not found",
    "Permission denied"
};

// Good: Use enums for error codes
typedef enum {
    SUCCESS = 0,
    ERROR_INVALID_ARG,
    ERROR_MEMORY,
    ERROR_FILE_NOT_FOUND,
    ERROR_PERMISSION_DENIED
} error_code_t;

// Good: Use typedef for complex types
typedef struct {
    char* data;
    size_t size;
    size_t capacity;
} string_buffer_t;

// Good: Function prototypes with proper documentation
/**
 * Creates a new string buffer with initial capacity
 * @param initial_capacity Initial capacity of the buffer
 * @return Pointer to new string buffer, or NULL on failure
 */
string_buffer_t* string_buffer_create(size_t initial_capacity);

/**
 * Appends a string to the buffer
 * @param buffer Pointer to string buffer
 * @param str String to append
 * @return SUCCESS on success, error code on failure
 */
error_code_t string_buffer_append(string_buffer_t* buffer, const char* str);

/**
 * Frees the string buffer and its resources
 * @param buffer Pointer to string buffer to free
 */
void string_buffer_destroy(string_buffer_t* buffer);

// Good: Input validation function
static bool is_valid_string(const char* str) {
    return str != NULL && strlen(str) > 0;
}

// Good: Safe memory allocation with error checking
static void* safe_malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }
    
    void* ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for %zu bytes\n", size);
    }
    
    return ptr;
}

// Good: Safe memory reallocation
static void* safe_realloc(void* ptr, size_t new_size) {
    if (new_size == 0) {
        free(ptr);
        return NULL;
    }
    
    void* new_ptr = realloc(ptr, new_size);
    if (new_ptr == NULL) {
        fprintf(stderr, "Error: Memory reallocation failed for %zu bytes\n", new_size);
        free(ptr);  // Free original pointer on failure
    }
    
    return new_ptr;
}

// Implementation of string buffer functions
string_buffer_t* string_buffer_create(size_t initial_capacity) {
    // Good: Validate input
    if (initial_capacity == 0) {
        initial_capacity = 16;  // Default capacity
    }
    
    // Good: Allocate structure first
    string_buffer_t* buffer = safe_malloc(sizeof(string_buffer_t));
    if (buffer == NULL) {
        return NULL;
    }
    
    // Good: Initialize all members
    buffer->data = safe_malloc(initial_capacity);
    if (buffer->data == NULL) {
        free(buffer);
        return NULL;
    }
    
    buffer->size = 0;
    buffer->capacity = initial_capacity;
    buffer->data[0] = '\0';  // Null-terminate empty string
    
    return buffer;
}

error_code_t string_buffer_append(string_buffer_t* buffer, const char* str) {
    // Good: Validate all inputs
    if (buffer == NULL || !is_valid_string(str)) {
        return ERROR_INVALID_ARG;
    }
    
    size_t str_len = strlen(str);
    size_t new_size = buffer->size + str_len + 1;  // +1 for null terminator
    
    // Good: Check for overflow
    if (new_size < buffer->size) {  // Overflow check
        return ERROR_INVALID_ARG;
    }
    
    // Good: Resize buffer if needed
    if (new_size > buffer->capacity) {
        size_t new_capacity = buffer->capacity * 2;
        while (new_capacity < new_size) {
            if (new_capacity > SIZE_MAX / 2) {  // Overflow check
                return ERROR_MEMORY;
            }
            new_capacity *= 2;
        }
        
        char* new_data = safe_realloc(buffer->data, new_capacity);
        if (new_data == NULL) {
            return ERROR_MEMORY;
        }
        
        buffer->data = new_data;
        buffer->capacity = new_capacity;
    }
    
    // Good: Safe string operations
    strcat(buffer->data, str);
    buffer->size = new_size - 1;  // -1 because we don't count null terminator
    
    return SUCCESS;
}

void string_buffer_destroy(string_buffer_t* buffer) {
    if (buffer != NULL) {
        free(buffer->data);
        free(buffer);
    }
}

// Good: Resource management with RAII-like pattern
typedef struct {
    FILE* file;
    bool is_open;
} file_handle_t;

file_handle_t* file_handle_open(const char* filename, const char* mode) {
    if (filename == NULL || mode == NULL) {
        return NULL;
    }
    
    file_handle_t* handle = safe_malloc(sizeof(file_handle_t));
    if (handle == NULL) {
        return NULL;
    }
    
    handle->file = fopen(filename, mode);
    if (handle->file == NULL) {
        free(handle);
        return NULL;
    }
    
    handle->is_open = true;
    return handle;
}

void file_handle_close(file_handle_t* handle) {
    if (handle != NULL && handle->is_open) {
        fclose(handle->file);
        handle->is_open = false;
    }
    free(handle);
}

// Good: Error handling with proper error codes
error_code_t safe_file_read(const char* filename, char** content, size_t* size) {
    if (filename == NULL || content == NULL || size == NULL) {
        return ERROR_INVALID_ARG;
    }
    
    file_handle_t* handle = file_handle_open(filename, "rb");
    if (handle == NULL) {
        return ERROR_FILE_NOT_FOUND;
    }
    
    // Get file size
    if (fseek(handle->file, 0, SEEK_END) != 0) {
        file_handle_close(handle);
        return ERROR_FILE_NOT_FOUND;
    }
    
    long file_size = ftell(handle->file);
    if (file_size < 0) {
        file_handle_close(handle);
        return ERROR_FILE_NOT_FOUND;
    }
    
    if (fseek(handle->file, 0, SEEK_SET) != 0) {
        file_handle_close(handle);
        return ERROR_FILE_NOT_FOUND;
    }
    
    // Allocate buffer
    *content = safe_malloc(file_size + 1);
    if (*content == NULL) {
        file_handle_close(handle);
        return ERROR_MEMORY;
    }
    
    // Read file
    size_t bytes_read = fread(*content, 1, file_size, handle->file);
    if (bytes_read != (size_t)file_size) {
        free(*content);
        file_handle_close(handle);
        return ERROR_FILE_NOT_FOUND;
    }
    
    (*content)[file_size] = '\0';  // Null-terminate
    *size = file_size;
    
    file_handle_close(handle);
    return SUCCESS;
}

// Good: Defensive programming
int safe_string_to_int(const char* str, int* result) {
    if (str == NULL || result == NULL) {
        return -1;
    }
    
    // Check for empty string
    if (str[0] == '\0') {
        return -1;
    }
    
    // Check for valid characters
    const char* p = str;
    if (*p == '-' || *p == '+') {
        p++;
    }
    
    if (*p == '\0') {  // Only sign character
        return -1;
    }
    
    while (*p != '\0') {
        if (*p < '0' || *p > '9') {
            return -1;
        }
        p++;
    }
    
    // Use strtol for safe conversion
    char* endptr;
    long value = strtol(str, &endptr, 10);
    
    // Check for conversion errors
    if (endptr == str || *endptr != '\0') {
        return -1;
    }
    
    // Check for overflow
    if (value < INT_MIN || value > INT_MAX) {
        return -1;
    }
    
    *result = (int)value;
    return 0;
}

// Good: Memory leak detection helper
#ifdef DEBUG
static size_t allocated_bytes = 0;

void* debug_malloc(size_t size) {
    void* ptr = malloc(size);
    if (ptr != NULL) {
        allocated_bytes += size;
        printf("DEBUG: Allocated %zu bytes, total: %zu\n", size, allocated_bytes);
    }
    return ptr;
}

void debug_free(void* ptr) {
    if (ptr != NULL) {
        free(ptr);
        printf("DEBUG: Freed memory\n");
    }
}
#else
#define debug_malloc malloc
#define debug_free free
#endif

// Good: Performance measurement
#include <time.h>

typedef struct {
    clock_t start_time;
    clock_t end_time;
} timer_t;

void timer_start(timer_t* timer) {
    timer->start_time = clock();
}

void timer_stop(timer_t* timer) {
    timer->end_time = clock();
}

double timer_elapsed_ms(timer_t* timer) {
    return ((double)(timer->end_time - timer->start_time) / CLOCKS_PER_SEC) * 1000.0;
}

// Good: Example of proper resource management
void demonstrate_best_practices(void) {
    printf("=== C Best Practices Demonstration ===\n");
    
    // 1. Proper error handling
    printf("\n1. Error Handling:\n");
    int result;
    if (safe_string_to_int("123", &result) == 0) {
        printf("Successfully converted '123' to %d\n", result);
    } else {
        printf("Failed to convert '123'\n");
    }
    
    if (safe_string_to_int("abc", &result) == 0) {
        printf("Successfully converted 'abc' to %d\n", result);
    } else {
        printf("Failed to convert 'abc' (expected)\n");
    }
    
    // 2. String buffer usage
    printf("\n2. String Buffer Usage:\n");
    string_buffer_t* buffer = string_buffer_create(10);
    if (buffer != NULL) {
        string_buffer_append(buffer, "Hello, ");
        string_buffer_append(buffer, "World!");
        printf("Buffer content: %s\n", buffer->data);
        printf("Buffer size: %zu, capacity: %zu\n", buffer->size, buffer->capacity);
        string_buffer_destroy(buffer);
    }
    
    // 3. Performance measurement
    printf("\n3. Performance Measurement:\n");
    timer_t timer;
    timer_start(&timer);
    
    // Simulate some work
    volatile int sum = 0;
    for (int i = 0; i < 1000000; i++) {
        sum += i;
    }
    
    timer_stop(&timer);
    printf("Loop took %.2f ms\n", timer_elapsed_ms(&timer));
    
    // 4. Memory management
    printf("\n4. Memory Management:\n");
    char* test_data = debug_malloc(100);
    if (test_data != NULL) {
        strcpy(test_data, "Test data");
        printf("Allocated and used: %s\n", test_data);
        debug_free(test_data);
    }
    
    // 5. Defensive programming
    printf("\n5. Defensive Programming:\n");
    const char* test_strings[] = {"123", "abc", "", NULL, "999999999999999999"};
    for (int i = 0; i < 5; i++) {
        int value;
        if (safe_string_to_int(test_strings[i], &value) == 0) {
            printf("'%s' -> %d\n", test_strings[i] ? test_strings[i] : "NULL", value);
        } else {
            printf("'%s' -> conversion failed\n", test_strings[i] ? test_strings[i] : "NULL");
        }
    }
}

// Good: Cleanup function for global resources
void cleanup_resources(void) {
    // Cleanup any global resources
    printf("Cleaning up resources...\n");
}

int main(void) {
    printf("C Best Practices Tutorial\n");
    printf("=========================\n");
    
    // Good: Register cleanup function
    atexit(cleanup_resources);
    
    demonstrate_best_practices();
    
    printf("\n=== Best Practices Summary ===\n");
    printf("1. Always validate input parameters\n");
    printf("2. Check return values from all functions\n");
    printf("3. Use const for read-only data\n");
    printf("4. Initialize all variables\n");
    printf("5. Free all allocated memory\n");
    printf("6. Use meaningful variable names\n");
    printf("7. Add proper error handling\n");
    printf("8. Use defensive programming techniques\n");
    printf("9. Document your functions\n");
    printf("10. Test edge cases and error conditions\n");
    
    return 0;
}

