# Go Tutorial: Language Essentials and Standard Library (Go 1.21+)

## Quick start

```bash
go run go/main.go
```

## Language basics

- Types: bool, numeric, string, arrays, slices, maps, structs
- Pointers (no pointer arithmetic), methods with value/pointer receivers
- Interfaces and structural typing; embedding
- Control flow: if, for, switch, type switch, defer, panic/recover
- Concurrency: goroutines, channels (unbuffered/buffered), `select`
- Modules and package layout

## Standard library highlights

- Text: `strings`, `bytes`, `regexp`, `strconv`
- I/O: `io`, `os`, `bufio`, `path/filepath`
- Time/ctx: `time`, `context`
- Net: `net/http`, `net`, `encoding/json`
- Sort/collections: `sort`, `container/heap`
- Testing: `testing`, benchmarks, examples

## Idioms

- Return `(value, error)`; use `errors.Is/As`
- Use `defer` for cleanup
- Prefer slices over arrays; preallocate with `make` when size known

See `go/main.go` for runnable examples.