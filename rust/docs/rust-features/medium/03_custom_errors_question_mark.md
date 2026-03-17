# Medium: Custom Errors and `?` Propagation

## What this feature is

Rust error handling is explicit and type-driven. Instead of exceptions, you return `Result<T, E>`. Creating domain-specific error enums gives callers precise failure reasons and supports structured recovery.

The `?` operator propagates errors ergonomically while preserving static types.

## Prerequisites

- Enums and `Result`
- Traits (`Debug`, `Display`)

## Self-contained example

```rust
use std::error::Error;
use std::fmt;

#[derive(Debug)]
enum AppError {
    EmptyInput,
    Parse(std::num::ParseIntError),
}

impl fmt::Display for AppError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            AppError::EmptyInput => write!(f, "input was empty"),
            AppError::Parse(e) => write!(f, "parse error: {e}"),
        }
    }
}

impl Error for AppError {}

fn parse_nonzero(input: &str) -> Result<u32, AppError> {
    if input.trim().is_empty() {
        return Err(AppError::EmptyInput);
    }
    let value: u32 = input.parse().map_err(AppError::Parse)?;
    if value == 0 {
        return Err(AppError::EmptyInput);
    }
    Ok(value)
}

fn main() {
    println!("{:?}", parse_nonzero("7"));
    println!("{:?}", parse_nonzero("0"));
}
```

## Best practices

- Keep error variants meaningful and actionable.
- Preserve source error context when converting.
- Decide where to convert to user-facing strings (usually near boundaries).

## Pitfalls

- Erasing useful context too early.
- Using panic for expected error paths.
