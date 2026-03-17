# Easy: Ownership, Borrowing, and Slices

## What this feature is

Ownership is Rust’s core memory-safety model:

- Every value has one owner.
- Values are dropped when the owner goes out of scope.
- Move semantics transfer ownership by default.

Borrowing (`&T`, `&mut T`) lets you access data without taking ownership. Slices (`&str`, `&[T]`) are borrowed views over contiguous memory.

## Prerequisites

- Variables and functions
- `String` and arrays

## Self-contained example

```rust
fn first_word(input: &str) -> &str {
    input.split_whitespace().next().unwrap_or("")
}

fn append_exclamation(text: &mut String) {
    text.push('!');
}

fn main() {
    let original = String::from("hello rust");
    let borrowed: &str = &original; // immutable borrow
    println!("borrowed={borrowed}");

    let mut changed = original.clone(); // clone when separate ownership is needed
    append_exclamation(&mut changed); // mutable borrow

    println!("first_word={}", first_word(&changed));
    println!("changed={changed}");
}
```

## Best practices

- Accept `&str` in APIs unless ownership is required.
- Keep mutable borrow scope short.
- Clone intentionally, not as a reflex.

## Pitfalls

- Using a value after move.
- Attempting simultaneous mutable and immutable borrows.
- Returning references to temporary values.
