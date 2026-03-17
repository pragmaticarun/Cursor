# Easy: Variables, Mutability, and Shadowing

## What this feature is

Rust variables are immutable by default. This design nudges you toward safer code by preventing accidental state changes. When you intentionally need mutation, you opt in with `mut`.

Shadowing lets you reuse the same variable name with a new binding (`let x = ...`). Unlike mutation, shadowing can also change type and is often used to represent staged transformations.

## Prerequisites

- Rust installed (`rustc`, `cargo`)
- Basic command line usage

## Self-contained example

```rust
fn main() {
    let x = 2;
    let x = x + 3; // shadowed: new immutable binding

    let mut y = 10; // mutable binding
    y += x;

    let spaces = "   ";
    let spaces = spaces.len(); // shadowing can change type (&str -> usize)

    println!("x={x}, y={y}, spaces={spaces}");
}
```

## Best practices

- Start immutable and upgrade to `mut` only when necessary.
- Use shadowing to represent transformation steps cleanly.
- Keep scope small so variables are easy to reason about.

## Pitfalls

- Confusing shadowing with mutation.
- Overusing `mut`, which can hide logic bugs.
- Reusing names too aggressively and reducing readability.
