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

---

## Advanced topics

- Concurrency patterns: worker pools, context cancellation, fan-in/out
- `sync` package: `WaitGroup`, `Mutex`, `RWMutex`, `Cond`, `Once`
- `atomic` operations for low-level concurrency
- Generics (Go 1.18+): type parameters, constraints, comparable
- `errors.Join` (1.20) and wrapping with `%w`
- Profiling with `pprof`, tracing, and benchmarking practices
- `io.Reader`/`Writer` composition; custom `net/http` middleware

## References

- Go tour: `https://go.dev/tour/`
- Effective Go: `https://go.dev/doc/effective_go`