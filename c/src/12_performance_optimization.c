#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#ifdef __AVX2__
#include <immintrin.h>  // For SIMD instructions
#endif

/**
 * C Performance Optimization Tutorial
 * Demonstrates optimization techniques, SIMD, and performance best practices
 */

// Performance measurement utilities
typedef struct {
    struct timespec start;
    struct timespec end;
} perf_timer_t;

void perf_timer_start(perf_timer_t* timer) {
    timer->start.tv_sec = 0;
    timer->start.tv_nsec = 0;
    // Use clock() as fallback
    timer->start.tv_sec = clock() / CLOCKS_PER_SEC;
    timer->start.tv_nsec = (clock() % CLOCKS_PER_SEC) * (1000000000 / CLOCKS_PER_SEC);
}

void perf_timer_end(perf_timer_t* timer) {
    timer->end.tv_sec = 0;
    timer->end.tv_nsec = 0;
    // Use clock() as fallback
    timer->end.tv_sec = clock() / CLOCKS_PER_SEC;
    timer->end.tv_nsec = (clock() % CLOCKS_PER_SEC) * (1000000000 / CLOCKS_PER_SEC);
}

double perf_timer_elapsed_ns(perf_timer_t* timer) {
    long seconds = timer->end.tv_sec - timer->start.tv_sec;
    long nanoseconds = timer->end.tv_nsec - timer->start.tv_nsec;
    return seconds * 1e9 + nanoseconds;
}

// Cache-friendly data structures
typedef struct {
    int id;
    char name[32];
    float value;
    int flags;
} cache_friendly_t;

typedef struct {
    int* ids;
    char* names;
    float* values;
    int* flags;
    size_t count;
} cache_unfriendly_t;

// SIMD-optimized functions
void vector_add_simple(const float* a, const float* b, float* result, size_t count) {
    for (size_t i = 0; i < count; i++) {
        result[i] = a[i] + b[i];
    }
}

#ifdef __AVX2__
void vector_add_simd(const float* a, const float* b, float* result, size_t count) {
    size_t simd_count = count & ~7;  // Process 8 elements at a time
    
    for (size_t i = 0; i < simd_count; i += 8) {
        __m256 va = _mm256_load_ps(&a[i]);
        __m256 vb = _mm256_load_ps(&b[i]);
        __m256 vresult = _mm256_add_ps(va, vb);
        _mm256_store_ps(&result[i], vresult);
    }
    
    // Handle remaining elements
    for (size_t i = simd_count; i < count; i++) {
        result[i] = a[i] + b[i];
    }
}
#endif

// Memory alignment optimization
void demonstrate_memory_alignment(void) {
    printf("=== Memory Alignment Optimization ===\n");
    
    const size_t count = 1000000;
    
    // Unaligned allocation
    float* unaligned = malloc(count * sizeof(float));
    
    // Aligned allocation
    float* aligned = aligned_alloc(32, count * sizeof(float));
    
    if (unaligned == NULL || aligned == NULL) {
        printf("Memory allocation failed\n");
        free(unaligned);
        free(aligned);
        return;
    }
    
    // Initialize data
    for (size_t i = 0; i < count; i++) {
        unaligned[i] = (float)i;
        aligned[i] = (float)i;
    }
    
    // Test performance
    perf_timer_t timer;
    double unaligned_time, aligned_time;
    
    // Test unaligned access
    perf_timer_start(&timer);
    volatile float sum1 = 0.0f;
    for (size_t i = 0; i < count; i++) {
        sum1 += unaligned[i];
    }
    perf_timer_end(&timer);
    unaligned_time = perf_timer_elapsed_ns(&timer);
    
    // Test aligned access
    perf_timer_start(&timer);
    volatile float sum2 = 0.0f;
    for (size_t i = 0; i < count; i++) {
        sum2 += aligned[i];
    }
    perf_timer_end(&timer);
    aligned_time = perf_timer_elapsed_ns(&timer);
    
    printf("Unaligned access time: %.2f ns\n", unaligned_time);
    printf("Aligned access time: %.2f ns\n", aligned_time);
    printf("Speedup: %.2fx\n", unaligned_time / aligned_time);
    printf("Sums: %.2f vs %.2f\n", sum1, sum2);
    
    free(unaligned);
    free(aligned);
}

// Cache optimization
void demonstrate_cache_optimization(void) {
    printf("\n=== Cache Optimization ===\n");
    
    const size_t size = 1000;
    const size_t iterations = 1000;
    
    // Cache-friendly: array of structs
    cache_friendly_t* friendly = malloc(size * sizeof(cache_friendly_t));
    
    // Cache-unfriendly: struct of arrays
    cache_unfriendly_t unfriendly = {
        .ids = malloc(size * sizeof(int)),
        .names = malloc(size * 32),
        .values = malloc(size * sizeof(float)),
        .flags = malloc(size * sizeof(int)),
        .count = size
    };
    
    if (friendly == NULL || unfriendly.ids == NULL || unfriendly.names == NULL ||
        unfriendly.values == NULL || unfriendly.flags == NULL) {
        printf("Memory allocation failed\n");
        free(friendly);
        free(unfriendly.ids);
        free(unfriendly.names);
        free(unfriendly.values);
        free(unfriendly.flags);
        return;
    }
    
    // Initialize data
    for (size_t i = 0; i < size; i++) {
        friendly[i].id = i;
        friendly[i].value = (float)i;
        friendly[i].flags = i % 2;
        
        unfriendly.ids[i] = i;
        unfriendly.values[i] = (float)i;
        unfriendly.flags[i] = i % 2;
    }
    
    perf_timer_t timer;
    double friendly_time, unfriendly_time;
    
    // Test cache-friendly access
    perf_timer_start(&timer);
    volatile float sum1 = 0.0f;
    for (size_t iter = 0; iter < iterations; iter++) {
        for (size_t i = 0; i < size; i++) {
            if (friendly[i].flags) {
                sum1 += friendly[i].value;
            }
        }
    }
    perf_timer_end(&timer);
    friendly_time = perf_timer_elapsed_ns(&timer);
    
    // Test cache-unfriendly access
    perf_timer_start(&timer);
    volatile float sum2 = 0.0f;
    for (size_t iter = 0; iter < iterations; iter++) {
        for (size_t i = 0; i < size; i++) {
            if (unfriendly.flags[i]) {
                sum2 += unfriendly.values[i];
            }
        }
    }
    perf_timer_end(&timer);
    unfriendly_time = perf_timer_elapsed_ns(&timer);
    
    printf("Cache-friendly time: %.2f ns\n", friendly_time);
    printf("Cache-unfriendly time: %.2f ns\n", unfriendly_time);
    printf("Speedup: %.2fx\n", unfriendly_time / friendly_time);
    printf("Sums: %.2f vs %.2f\n", sum1, sum2);
    
    free(friendly);
    free(unfriendly.ids);
    free(unfriendly.names);
    free(unfriendly.values);
    free(unfriendly.flags);
}

// SIMD optimization
void demonstrate_simd_optimization(void) {
    printf("\n=== SIMD Optimization ===\n");
    
    const size_t count = 1000000;
    float* a = aligned_alloc(32, count * sizeof(float));
    float* b = aligned_alloc(32, count * sizeof(float));
    float* result1 = aligned_alloc(32, count * sizeof(float));
    float* result2 = aligned_alloc(32, count * sizeof(float));
    
    if (a == NULL || b == NULL || result1 == NULL || result2 == NULL) {
        printf("Memory allocation failed\n");
        free(a);
        free(b);
        free(result1);
        free(result2);
        return;
    }
    
    // Initialize data
    for (size_t i = 0; i < count; i++) {
        a[i] = (float)i;
        b[i] = (float)(i * 2);
    }
    
    perf_timer_t timer;
    double simple_time;
    
    // Test simple vector addition
    perf_timer_start(&timer);
    vector_add_simple(a, b, result1, count);
    perf_timer_end(&timer);
    simple_time = perf_timer_elapsed_ns(&timer);
    
#ifdef __AVX2__
    // Test SIMD vector addition
    perf_timer_start(&timer);
    vector_add_simd(a, b, result2, count);
    perf_timer_end(&timer);
    double simd_time = perf_timer_elapsed_ns(&timer);
    
    printf("Simple vector addition time: %.2f ns\n", simple_time);
    printf("SIMD vector addition time: %.2f ns\n", simd_time);
    printf("Speedup: %.2fx\n", simple_time / simd_time);
    
    // Verify results are the same
    bool results_match = true;
    for (size_t i = 0; i < count; i++) {
        if (result1[i] != result2[i]) {
            results_match = false;
            break;
        }
    }
    printf("Results match: %s\n", results_match ? "Yes" : "No");
#else
    printf("SIMD not available (AVX2 not supported)\n");
    printf("Simple vector addition time: %.2f ns\n", simple_time);
#endif
    
    free(a);
    free(b);
    free(result1);
    free(result2);
}

// Loop optimization
void demonstrate_loop_optimization(void) {
    printf("\n=== Loop Optimization ===\n");
    
    const size_t size = 1000;
    int* data = malloc(size * sizeof(int));
    if (data == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    
    // Initialize data
    for (size_t i = 0; i < size; i++) {
        data[i] = (int)i;
    }
    
    perf_timer_t timer;
    double unoptimized_time, optimized_time;
    
    // Unoptimized loop
    perf_timer_start(&timer);
    volatile int sum1 = 0;
    for (size_t i = 0; i < size; i++) {
        sum1 += data[i] * 2;
    }
    perf_timer_end(&timer);
    unoptimized_time = perf_timer_elapsed_ns(&timer);
    
    // Optimized loop (loop unrolling)
    perf_timer_start(&timer);
    volatile int sum2 = 0;
    size_t i;
    for (i = 0; i < size - 3; i += 4) {
        sum2 += data[i] * 2;
        sum2 += data[i + 1] * 2;
        sum2 += data[i + 2] * 2;
        sum2 += data[i + 3] * 2;
    }
    for (; i < size; i++) {
        sum2 += data[i] * 2;
    }
    perf_timer_end(&timer);
    optimized_time = perf_timer_elapsed_ns(&timer);
    
    printf("Unoptimized loop time: %.2f ns\n", unoptimized_time);
    printf("Optimized loop time: %.2f ns\n", optimized_time);
    printf("Speedup: %.2fx\n", unoptimized_time / optimized_time);
    printf("Sums: %d vs %d\n", sum1, sum2);
    
    free(data);
}

// Comparison function for qsort
int compare_ints_perf(const void* a, const void* b) {
    int ia = *(const int*)a;
    int ib = *(const int*)b;
    return (ia > ib) - (ia < ib);
}

// Branch prediction optimization
void demonstrate_branch_optimization(void) {
    printf("\n=== Branch Prediction Optimization ===\n");
    
    const size_t size = 1000000;
    int* data = malloc(size * sizeof(int));
    if (data == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    
    // Initialize with random data
    srand(42);
    for (size_t i = 0; i < size; i++) {
        data[i] = rand() % 100;
    }
    
    perf_timer_t timer;
    double unsorted_time, sorted_time;
    
    // Test with unsorted data (poor branch prediction)
    perf_timer_start(&timer);
    volatile int sum1 = 0;
    for (size_t i = 0; i < size; i++) {
        if (data[i] < 50) {
            sum1 += data[i];
        }
    }
    perf_timer_end(&timer);
    unsorted_time = perf_timer_elapsed_ns(&timer);
    
    // Sort data for better branch prediction
    qsort(data, size, sizeof(int), compare_ints_perf);
    
    // Test with sorted data (good branch prediction)
    perf_timer_start(&timer);
    volatile int sum2 = 0;
    for (size_t i = 0; i < size; i++) {
        if (data[i] < 50) {
            sum2 += data[i];
        }
    }
    perf_timer_end(&timer);
    sorted_time = perf_timer_elapsed_ns(&timer);
    
    printf("Unsorted data time: %.2f ns\n", unsorted_time);
    printf("Sorted data time: %.2f ns\n", sorted_time);
    printf("Speedup: %.2fx\n", unsorted_time / sorted_time);
    printf("Sums: %d vs %d\n", sum1, sum2);
    
    free(data);
}

// Function inlining optimization
static inline int square_inline(int x) {
    return x * x;
}

static int square_no_inline(int x) {
    return x * x;
}

void demonstrate_inlining_optimization(void) {
    printf("\n=== Function Inlining Optimization ===\n");
    
    const size_t count = 1000000;
    
    perf_timer_t timer;
    double inline_time, no_inline_time;
    
    // Test inlined function
    perf_timer_start(&timer);
    volatile int sum1 = 0;
    for (size_t i = 0; i < count; i++) {
        sum1 += square_inline((int)i);
    }
    perf_timer_end(&timer);
    inline_time = perf_timer_elapsed_ns(&timer);
    
    // Test non-inlined function
    perf_timer_start(&timer);
    volatile int sum2 = 0;
    for (size_t i = 0; i < count; i++) {
        sum2 += square_no_inline((int)i);
    }
    perf_timer_end(&timer);
    no_inline_time = perf_timer_elapsed_ns(&timer);
    
    printf("Inlined function time: %.2f ns\n", inline_time);
    printf("Non-inlined function time: %.2f ns\n", no_inline_time);
    printf("Speedup: %.2fx\n", no_inline_time / inline_time);
    printf("Sums: %d vs %d\n", sum1, sum2);
}

int main(void) {
    printf("C Performance Optimization Tutorial\n");
    printf("===================================\n");
    
    demonstrate_memory_alignment();
    demonstrate_cache_optimization();
    demonstrate_simd_optimization();
    demonstrate_loop_optimization();
    demonstrate_branch_optimization();
    demonstrate_inlining_optimization();
    
    printf("\n=== Optimization Summary ===\n");
    printf("1. Use aligned memory allocation for SIMD\n");
    printf("2. Organize data structures for cache efficiency\n");
    printf("3. Use SIMD instructions when available\n");
    printf("4. Unroll loops for small, hot loops\n");
    printf("5. Sort data to improve branch prediction\n");
    printf("6. Use inline functions for small, frequently called functions\n");
    printf("7. Profile before and after optimizations\n");
    printf("8. Use compiler optimizations (-O2, -O3)\n");
    printf("9. Consider data locality and access patterns\n");
    printf("10. Measure, don't guess!\n");
    
    return 0;
}
