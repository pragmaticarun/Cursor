#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>
#include <stdbool.h>

/**
 * C Debugging and Profiling Tutorial
 * Demonstrates debugging techniques, profiling, and performance analysis
 */

// Debug macros
#ifdef DEBUG
    #define DBG_PRINT(fmt, ...) fprintf(stderr, "DEBUG: " fmt "\n", ##__VA_ARGS__)
    #define DBG_ASSERT(expr) assert(expr)
#else
    #define DBG_PRINT(fmt, ...) ((void)0)
    #define DBG_ASSERT(expr) ((void)0)
#endif

// Performance counter
static unsigned long long performance_counter = 0;

// Signal handling for debugging
static jmp_buf debug_jump_buffer;

void signal_handler(int sig) {
    printf("\n=== Signal Received: %d ===\n", sig);
    
    // Print stack trace (simplified version)
    printf("Stack trace:\n");
    printf("  Signal received at program execution\n");
    
    // Jump back to debug point
    longjmp(debug_jump_buffer, 1);
}

// Memory debugging
#ifdef DEBUG
static size_t total_allocated = 0;
static size_t allocation_count = 0;

void* debug_malloc(size_t size) {
    void* ptr = malloc(size);
    if (ptr != NULL) {
        total_allocated += size;
        allocation_count++;
        DBG_PRINT("Allocated %zu bytes at %p (total: %zu, count: %zu)", 
                  size, ptr, total_allocated, allocation_count);
    }
    return ptr;
}

void debug_free(void* ptr) {
    if (ptr != NULL) {
        free(ptr);
        DBG_PRINT("Freed memory at %p", ptr);
    }
}

void debug_memory_stats(void) {
    printf("Memory Statistics:\n");
    printf("  Total allocated: %zu bytes\n", total_allocated);
    printf("  Allocation count: %zu\n", allocation_count);
}
#else
#define debug_malloc malloc
#define debug_free free
#define debug_memory_stats() ((void)0)
#endif

// Performance profiling
typedef struct {
    char name[64];
    clock_t start_time;
    clock_t total_time;
    int call_count;
} profiler_t;

static profiler_t profilers[10];
static int profiler_count = 0;

void profiler_start(const char* name) {
    if (profiler_count >= 10) {
        printf("Warning: Too many profilers\n");
        return;
    }
    
    strncpy(profilers[profiler_count].name, name, sizeof(profilers[profiler_count].name) - 1);
    profilers[profiler_count].name[sizeof(profilers[profiler_count].name) - 1] = '\0';
    profilers[profiler_count].start_time = clock();
    profilers[profiler_count].call_count = 1;
    profilers[profiler_count].total_time = 0;
    profiler_count++;
}

void profiler_end(const char* name) {
    for (int i = 0; i < profiler_count; i++) {
        if (strcmp(profilers[i].name, name) == 0) {
            clock_t end_time = clock();
            profilers[i].total_time += (end_time - profilers[i].start_time);
            break;
        }
    }
}

void profiler_print_stats(void) {
    printf("\n=== Profiling Statistics ===\n");
    for (int i = 0; i < profiler_count; i++) {
        double time_ms = ((double)profilers[i].total_time / CLOCKS_PER_SEC) * 1000.0;
        printf("%s: %.2f ms (%d calls)\n", 
               profilers[i].name, time_ms, profilers[i].call_count);
    }
}

// Example functions for profiling
void slow_function(void) {
    profiler_start("slow_function");
    
    // Simulate slow operation
    volatile int sum = 0;
    for (int i = 0; i < 1000000; i++) {
        sum += i;
    }
    
    profiler_end("slow_function");
}

void fast_function(void) {
    profiler_start("fast_function");
    
    // Simulate fast operation
    volatile int sum = 0;
    for (int i = 0; i < 1000; i++) {
        sum += i;
    }
    
    profiler_end("fast_function");
}

// Memory leak detection
void demonstrate_memory_leaks(void) {
    printf("\n=== Memory Leak Detection ===\n");
    
    debug_memory_stats();
    
    // Intentionally create memory leak
    char* leak1 = debug_malloc(100);
    char* leak2 = debug_malloc(200);
    
    debug_memory_stats();
    
    // Free one, leak the other
    debug_free(leak1);
    // leak2 is intentionally not freed (memory leak)
    (void)leak2;  // Suppress unused variable warning
    
    debug_memory_stats();
}

// Buffer overflow detection
void demonstrate_buffer_overflow(void) {
    printf("\n=== Buffer Overflow Detection ===\n");
    
    char buffer[10];
    
    // Safe operation
    strncpy(buffer, "Hello", sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    printf("Safe copy: %s\n", buffer);
    
    // Dangerous operation (commented out to prevent actual overflow)
    // strcpy(buffer, "This is a very long string that will overflow the buffer");
    
    printf("Buffer overflow detection would catch the above operation\n");
}

// Assertion testing
void demonstrate_assertions(void) {
    printf("\n=== Assertion Testing ===\n");
    
    int value = 42;
    
    // These assertions will pass
    DBG_ASSERT(value > 0);
    DBG_ASSERT(value == 42);
    
    printf("Assertions passed\n");
    
    // This assertion would fail in debug mode
    // DBG_ASSERT(value < 0);
    
    (void)value;  // Suppress unused variable warning
}

// Signal handling and recovery
void demonstrate_signal_handling(void) {
    printf("\n=== Signal Handling ===\n");
    
    // Set up signal handler
    signal(SIGINT, signal_handler);
    signal(SIGSEGV, signal_handler);
    
    printf("Signal handlers installed. Press Ctrl+C to test.\n");
    
    if (setjmp(debug_jump_buffer) == 0) {
        printf("In signal handling test. Press Ctrl+C to trigger signal.\n");
        printf("Waiting 3 seconds...\n");
        sleep(3);
        printf("No signal received, continuing...\n");
    } else {
        printf("Signal received and handled!\n");
    }
}

// Comparison function for qsort
int compare_ints_debug(const void* a, const void* b) {
    int ia = *(const int*)a;
    int ib = *(const int*)b;
    return (ia > ib) - (ia < ib);
}

// Performance analysis
void demonstrate_performance_analysis(void) {
    printf("\n=== Performance Analysis ===\n");
    
    // Test different algorithms
    const int size = 10000;
    int* data = debug_malloc(size * sizeof(int));
    if (data == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    
    // Initialize data
    for (int i = 0; i < size; i++) {
        data[i] = rand() % 1000;
    }
    
    // Test bubble sort
    profiler_start("bubble_sort");
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (data[j] > data[j + 1]) {
                int temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
    profiler_end("bubble_sort");
    
    // Test quicksort (simplified)
    profiler_start("quicksort");
    // Reset data
    for (int i = 0; i < size; i++) {
        data[i] = rand() % 1000;
    }
    
    // Simple quicksort implementation
    qsort(data, size, sizeof(int), compare_ints_debug);
    profiler_end("quicksort");
    
    debug_free(data);
    profiler_print_stats();
}

// Debugging with breakpoints (simulated)
void demonstrate_debugging_techniques(void) {
    printf("\n=== Debugging Techniques ===\n");
    
    int x = 10;
    int y = 20;
    int result = 0;
    
    // Simulate debugging session
    printf("Step 1: x = %d, y = %d\n", x, y);
    
    // Simulate breakpoint
    printf("Breakpoint: Check values before operation\n");
    
    result = x + y;
    printf("Step 2: result = x + y = %d\n", result);
    
    // Simulate conditional breakpoint
    if (result > 25) {
        printf("Conditional breakpoint: result > 25\n");
    }
    
    // Simulate watchpoint
    x = 30;
    printf("Watchpoint: x changed to %d\n", x);
    
    result = x + y;
    printf("Step 3: result = x + y = %d\n", result);
}

// Memory corruption detection
void demonstrate_memory_corruption(void) {
    printf("\n=== Memory Corruption Detection ===\n");
    
    char* buffer = debug_malloc(20);
    if (buffer == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    
    // Safe operations
    strcpy(buffer, "Hello");
    printf("Buffer: %s\n", buffer);
    
    // Simulate memory corruption (commented out to prevent actual corruption)
    // buffer[25] = 'X';  // This would corrupt memory
    
    printf("Memory corruption detection would catch out-of-bounds writes\n");
    
    debug_free(buffer);
}

// Resource leak detection
void demonstrate_resource_leaks(void) {
    printf("\n=== Resource Leak Detection ===\n");
    
    FILE* file = fopen("test_file.txt", "w");
    if (file == NULL) {
        printf("Failed to open file\n");
        return;
    }
    
    fprintf(file, "Test data\n");
    
    // Intentionally not closing file to demonstrate leak
    // fclose(file);
    
    printf("Resource leak: File handle not closed\n");
    printf("Use tools like valgrind to detect such leaks\n");
}

// Performance counter
void increment_performance_counter(void) {
    performance_counter++;
}

void print_performance_counter(void) {
    printf("Performance counter: %llu\n", performance_counter);
}

int main(void) {
    printf("C Debugging and Profiling Tutorial\n");
    printf("===================================\n");
    
    // Set up debugging environment
    printf("Debug mode: %s\n", 
#ifdef DEBUG
           "ENABLED"
#else
           "DISABLED"
#endif
    );
    
    demonstrate_memory_leaks();
    demonstrate_buffer_overflow();
    demonstrate_assertions();
    demonstrate_signal_handling();
    demonstrate_performance_analysis();
    demonstrate_debugging_techniques();
    demonstrate_memory_corruption();
    demonstrate_resource_leaks();
    
    // Print final statistics
    debug_memory_stats();
    print_performance_counter();
    
    printf("\n=== Debugging Tools Summary ===\n");
    printf("1. gdb - GNU Debugger\n");
    printf("2. valgrind - Memory error detector\n");
    printf("3. AddressSanitizer - Memory error detector\n");
    printf("4. gprof - Profiler\n");
    printf("5. strace - System call tracer\n");
    printf("6. ltrace - Library call tracer\n");
    printf("7. perf - Performance analysis tool\n");
    
    return 0;
}

