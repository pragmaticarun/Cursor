# Easy: Modules, Collections, `Option`, and `Result`

## What this feature is

Modules organize code and control visibility (`pub`). Collections such as `Vec` and `String` are core dynamic data containers. `Option<T>` models optional data; `Result<T, E>` models fallible operations and forces explicit error handling.

## Prerequisites

- Functions and ownership basics
- Structs/enums basics

## Self-contained example

```rust
mod math {
    pub fn add(a: i32, b: i32) -> i32 {
        a + b
    }
}

fn parse_port(input: &str) -> Result<u16, String> {
    let port = input.parse::<u16>().map_err(|e| e.to_string())?;
    if port == 0 {
        return Err("port must be > 0".to_string());
    }
    Ok(port)
}

fn maybe_nth(values: &[i32], idx: usize) -> Option<i32> {
    values.get(idx).copied()
}

fn main() {
    let mut values = vec![3, 1, 2];
    values.sort();

    println!("sum={}", math::add(20, 22));
    println!("sorted={values:?}");
    println!("maybe_nth(10)={:?}", maybe_nth(&values, 10));
    println!("parse_port(8080)={:?}", parse_port("8080"));
}
```

## Best practices

- Keep modules focused by responsibility.
- Return `Option`/`Result` instead of panicking.
- Prefer `?` for clear error propagation.

## Pitfalls

- Excessive `unwrap()` in production code.
- Exposing too much module internals with broad `pub` usage.
