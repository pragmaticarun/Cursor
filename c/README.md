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

---

## Advanced language features and pitfalls

- Strict aliasing rules and `-fno-strict-aliasing` trade-offs
- Alignment and `aligned_alloc`; structure padding and `#pragma pack` risks
- Atomics and threads (C11): `<stdatomic.h>`, `<threads.h>` availability varies
- UB minefield: uninitialized reads, signed overflow, dangling pointers
- Preprocessor hygiene: include guards, macro pitfalls, function-like macros vs inline
- Error handling patterns: sentinel values, errno (`<errno.h>`), `perror`

## libc cookbook

- `getline` (POSIX) for safe line reading; `strtok_r` for re-entrant tokenizing
- `qsort`/`bsearch` comparators; stable sort requires custom implementation
- Temporary files: `tmpfile()` and `mkstemp()` (POSIX)
- Environment: `getenv`, `setenv` (POSIX), careful with thread safety

## Tools

- Compile with sanitizers: `-fsanitize=address,undefined`
- Static analysis: `clang-tidy`, `cppcheck`

## References

- ISO C draft: `http://www.open-std.org/jtc1/sc22/wg14/`
- C FAQ: `https://c-faq.com/`