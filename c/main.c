#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int cmp_ints(const void* a, const void* b) {
    int ia = *(const int*)a;
    int ib = *(const int*)b;
    return (ia > ib) - (ia < ib);
}

int main(void) {
    puts("=== C demo ===");

    // Strings and character processing
    const char* msg = "Hello, World!";
    int letters = 0;
    for (const char* p = msg; *p; ++p) if (isalpha((unsigned char)*p)) ++letters;
    printf("Letters: %d\n", letters);

    // Arrays and qsort
    int values[] = {5, 2, 9, 1, 5, 6};
    size_t n = sizeof values / sizeof *values;
    qsort(values, n, sizeof *values, cmp_ints);
    for (size_t i = 0; i < n; ++i) printf(i + 1 == n ? "%d\n" : "%d ", values[i]);

    // Dynamic memory
    char* buf = malloc(16);
    if (!buf) return 1;
    strcpy(buf, "C language");
    printf("buf: %s (len=%zu)\n", buf, strlen(buf));
    free(buf);

    return 0;
}