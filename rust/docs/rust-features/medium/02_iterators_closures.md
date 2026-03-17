# Medium: Iterators and Closures

## What this feature is

Iterators are lazy pipelines for processing sequences. Closures are anonymous functions that can capture environment state. Combined, they allow concise, expressive transformations while preserving type safety and performance.

## Prerequisites

- Vectors/collections
- Basic functions

## Self-contained example

```rust
fn main() {
    let values = vec![1, 2, 3, 4, 5, 6];

    let even_squares: Vec<i32> = values
        .into_iter()        // takes ownership
        .filter(|n| n % 2 == 0)
        .map(|n| n * n)
        .collect();

    println!("{even_squares:?}");
}
```

## Best practices

- Prefer iterator chains for transformation-heavy logic.
- Choose the right iterator flavor:
  - `iter()` for shared borrow
  - `iter_mut()` for mutable borrow
  - `into_iter()` for ownership move

## Pitfalls

- Accidentally moving values with `into_iter()`.
- Writing overly long chains that become hard to read (split into named steps).
