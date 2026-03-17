# Medium: Testing, Tooling, and Conditional Compilation

## What this feature is

Rust’s development loop is tightly integrated with Cargo:

- `cargo test` for correctness
- `cargo fmt` for formatting consistency
- `cargo clippy` for lint-driven quality improvements

Conditional compilation (`#[cfg(...)]`) lets you compile code only for certain platforms, features, or build modes.

## Prerequisites

- Cargo project basics
- Functions and modules

## Self-contained example

```rust
#[cfg(debug_assertions)]
fn build_mode() -> &'static str {
    "debug"
}

#[cfg(not(debug_assertions))]
fn build_mode() -> &'static str {
    "release"
}

fn add(a: i32, b: i32) -> i32 {
    a + b
}

fn main() {
    println!("mode={}", build_mode());
    println!("2 + 3 = {}", add(2, 3));
}

#[cfg(test)]
mod tests {
    use super::add;

    #[test]
    fn adds_numbers() {
        assert_eq!(add(2, 3), 5);
    }
}
```

## Best practices

- Run `fmt`, `clippy`, and tests before every commit.
- Gate platform-specific code with `cfg` instead of runtime checks.
- Keep tests deterministic and isolated.

## Pitfalls

- Forgetting to test both cfg paths.
- Treating clippy warnings as optional in critical code.
