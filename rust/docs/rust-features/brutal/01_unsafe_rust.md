# Brutal: Unsafe Rust

## What this feature is

Unsafe Rust is an escape hatch that allows operations the compiler cannot prove safe, such as dereferencing raw pointers, calling unsafe functions, and accessing mutable statics.

Important: `unsafe` does **not** disable borrowing rules globally. It only permits specific operations inside explicit blocks.

## Prerequisites

- Strong ownership and borrowing understanding
- Pointer fundamentals

## Self-contained example

```rust
fn main() {
    let mut value = 41;
    let ptr = &mut value as *mut i32;

    unsafe {
        // SAFETY:
        // ptr comes from a valid mutable reference to `value`
        // and is used while `value` is still alive.
        *ptr += 1;
    }

    println!("value={value}");
}
```

## Best practices

- Keep unsafe blocks as small as possible.
- Document safety invariants right above each unsafe use.
- Wrap unsafe internals behind safe APIs when possible.

## Pitfalls

- Creating dangling pointers.
- Violating aliasing assumptions (`&mut` uniqueness).
- Using unsafe for performance guesses without profiling.
