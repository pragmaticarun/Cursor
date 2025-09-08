#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <float.h>

/**
 * C Types Tutorial
 * Demonstrates basic C data types and their properties
 */

void demonstrate_integer_types(void) {
    printf("=== Integer Types ===\n");
    
    // Basic integer types
    char c = 'A';
    short s = 32767;
    int i = 2147483647;
    long l = 2147483647L;
    long long ll = 9223372036854775807LL;
    
    printf("char: %c (size: %zu bytes)\n", c, sizeof(c));
    printf("short: %hd (size: %zu bytes)\n", s, sizeof(s));
    printf("int: %d (size: %zu bytes)\n", i, sizeof(i));
    printf("long: %ld (size: %zu bytes)\n", l, sizeof(l));
    printf("long long: %lld (size: %zu bytes)\n", ll, sizeof(ll));
    
    // Fixed-width integer types
    int8_t i8 = 127;
    int16_t i16 = 32767;
    int32_t i32 = 2147483647;
    int64_t i64 = 9223372036854775807LL;
    
    printf("\nFixed-width integers:\n");
    printf("int8_t: %d (size: %zu bytes)\n", i8, sizeof(i8));
    printf("int16_t: %d (size: %zu bytes)\n", i16, sizeof(i16));
    printf("int32_t: %d (size: %zu bytes)\n", i32, sizeof(i32));
    printf("int64_t: %ld (size: %zu bytes)\n", i64, sizeof(i64));
    
    // Unsigned types
    unsigned char uc = 255;
    unsigned int ui = 4294967295U;
    size_t sz = sizeof(int);
    
    printf("\nUnsigned types:\n");
    printf("unsigned char: %u (size: %zu bytes)\n", uc, sizeof(uc));
    printf("unsigned int: %u (size: %zu bytes)\n", ui, sizeof(ui));
    printf("size_t: %zu (size: %zu bytes)\n", sz, sizeof(sz));
}

void demonstrate_floating_types(void) {
    printf("\n=== Floating Point Types ===\n");
    
    float f = 3.14159f;
    double d = 3.141592653589793;
    long double ld = 3.141592653589793238L;
    
    printf("float: %.6f (size: %zu bytes)\n", f, sizeof(f));
    printf("double: %.15f (size: %zu bytes)\n", d, sizeof(d));
    printf("long double: %.18Lf (size: %zu bytes)\n", ld, sizeof(ld));
    
    // Special floating point values
    float inf = 1.0f / 0.0f;
    float nan = 0.0f / 0.0f;
    
    printf("infinity: %f\n", inf);
    printf("NaN: %f\n", nan);
}

void demonstrate_boolean_type(void) {
    printf("\n=== Boolean Type ===\n");
    
    bool b1 = true;
    bool b2 = false;
    bool b3 = 42;  // Non-zero values are true
    
    printf("true: %d\n", b1);
    printf("false: %d\n", b2);
    printf("42 (as bool): %d\n", b3);
}

void demonstrate_type_limits(void) {
    printf("\n=== Type Limits ===\n");
    
    printf("INT_MAX: %d\n", INT_MAX);
    printf("INT_MIN: %d\n", INT_MIN);
    printf("UINT_MAX: %u\n", UINT_MAX);
    printf("LONG_MAX: %ld\n", LONG_MAX);
    printf("LLONG_MAX: %lld\n", LLONG_MAX);
    
    printf("FLT_MAX: %e\n", FLT_MAX);
    printf("FLT_MIN: %e\n", FLT_MIN);
    printf("DBL_MAX: %e\n", DBL_MAX);
    printf("DBL_MIN: %e\n", DBL_MIN);
}

void demonstrate_type_conversions(void) {
    printf("\n=== Type Conversions ===\n");
    
    int i = 42;
    float f = 3.14f;
    double d = 2.718;
    
    // Implicit conversions
    double result1 = i + f;  // int + float -> double
    int result2 = f;         // float -> int (truncation)
    
    printf("int + float = double: %d + %.2f = %.2f\n", i, f, result1);
    printf("float to int: %.2f -> %d\n", f, result2);
    
    // Explicit casting
    int result3 = (int)d;
    float result4 = (float)i;
    
    printf("Explicit cast double to int: %.2f -> %d\n", d, result3);
    printf("Explicit cast int to float: %d -> %.2f\n", i, result4);
}

int main(void) {
    printf("C Types Tutorial\n");
    printf("================\n");
    
    demonstrate_integer_types();
    demonstrate_floating_types();
    demonstrate_boolean_type();
    demonstrate_type_limits();
    demonstrate_type_conversions();
    
    return 0;
}

