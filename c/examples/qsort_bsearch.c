#include <stdio.h>
#include <stdlib.h>

static int cmp_int(const void* a, const void* b){
    int ia = *(const int*)a, ib = *(const int*)b;
    return (ia>ib) - (ia<ib);
}

int main(void){
    int a[] = {5,2,9,1,5,6};
    size_t n = sizeof a / sizeof *a;
    qsort(a, n, sizeof *a, cmp_int);
    int key = 5; int* p = bsearch(&key, a, n, sizeof *a, cmp_int);
    printf("sorted: ");
    for(size_t i=0;i<n;i++) printf("%d%c", a[i], i+1==n?'\n':' ');
    printf("found=%d at idx=%ld\n", *p, (long)(p - a));
    return 0;
}