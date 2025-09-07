# Rust Tutorial: Language Essentials and Standard Library (Edition 2021)

## Quick start

```bash
cd rust && cargo run
```

## Language basics

- Ownership/borrowing: move, copy, references (`&T` / `&mut T`)
- Lifetimes: elision, explicit lifetimes for APIs
- Enums and pattern matching; `match`, `if let`, `while let`
- Traits and generics; trait bounds; blanket impls
- Error handling: `Result<T, E>`, `?`, `thiserror` (external), `anyhow` (external)
- Iterators and closures; adapters and consumers

## Standard library highlights

- Collections: `Vec`, `VecDeque`, `HashMap`, `BTreeMap`
- I/O: `std::fs`, `std::io::{Read, Write, BufRead}`
- Time/threads: `std::time`, `std::thread`, `std::sync` (Mutex, Arc)
- Path/OS: `std::path::Path`, `std::env`

## Idioms

- Prefer iterators over indexing; avoid unnecessary clones
- Use `Option` for nullable values; `as_deref`, `as_ref` helpers
- `From`/`Into` conversions; `TryFrom`/`TryInto`

See `rust/src/main.rs` for runnable examples.