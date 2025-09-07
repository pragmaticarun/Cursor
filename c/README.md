# C Tutorial: Language Essentials and Standard Library (C11)

A focused guide to modern C (C11) with portable standard library usage.

## Quick start

```bash
# Compile and run
gcc -std=c11 -Wall -Wextra -O2 main.c -o app && ./app
```

## Language basics

- Primitives: integers (`int`, `long long`), floating (`double`), `char`, `_Bool`
- Pointers and arrays; pointer arithmetic
- `struct` and `enum`
- Control flow: `if`, `switch`, loops, `break`/`continue`
- Functions: prototypes, `static`, `inline`
- Lifetime: stack vs heap (`malloc`/`free`)

## Standard library highlights

- `<stdio.h>`: I/O (`printf`, `fgets`, `fopen`)
- `<stdlib.h>`: memory, conversions, sorting (`qsort`)
- `<string.h>`: string and memory utilities
- `<time.h>`: time utilities
- `<ctype.h>`: character classification

See `main.c` for a runnable demo.