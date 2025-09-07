# C Tutorial: Language Essentials and Standard Library (C11)

## Quick start

```bash
gcc -std=c11 -Wall -Wextra -O2 c/main.c -o c/app && ./c/app
```

## Language basics

- Types: fixed-width integers (`stdint.h`), floating types, `char`, `_Bool`
- Pointers and arrays; pointer arithmetic; `restrict`
- `struct`, `enum`, bitfields; `typedef`
- Control flow: `if`, `switch`, loops, `break`/`continue`
- Functions: prototypes, `static`, `inline`, variadic (`stdarg.h`)
- Lifetime: stack vs heap (`malloc`/`calloc`/`realloc`/`free`)
- `const` correctness; `volatile` for hardware/atomics context

## Standard library highlights

- I/O: `<stdio.h>` `printf`, `scanf`, `fopen`, `fgets`, `fprintf`
- Memory and utils: `<stdlib.h>` `malloc`, `free`, `qsort`, `bsearch`, `strtol`
- Strings/memory: `<string.h>` `memcpy`, `memset`, `strcmp`, `strncpy`
- Time: `<time.h>` `clock_gettime` (POSIX), `time`, `strftime`
- Character classification: `<ctype.h>` `isalpha`, `isdigit`
- Math: `<math.h>` `sqrt`, `pow`, `fabs`
- Fixed-width ints: `<stdint.h>`; limits: `<limits.h>`

## Guidance

- Always check pointers and return codes
- Avoid buffer overflows; use length-checked functions
- Keep interfaces simple; document ownership and lifetimes

See `c/main.c` for runnable examples.