# Brutal: FFI (Foreign Function Interface)

## What this feature is

FFI lets Rust call functions from other languages (commonly C) and expose Rust functions to external code. This is critical for system integration, legacy interop, and low-level runtime hooks.

Rust uses `extern "C"` to define ABI compatibility with C.

## Prerequisites

- Unsafe Rust basics
- Linking and C ABI awareness

## Self-contained example

```rust
unsafe extern "C" {
    fn abs(input: i32) -> i32;
}

fn main() {
    let value = -42;
    let absolute = unsafe {
        // SAFETY:
        // `abs` is provided by the C standard library and accepts i32-compatible int.
        abs(value)
    };
    println!("abs({value}) = {absolute}");
}
```

## Best practices

- Keep ABI boundaries narrow and well-documented.
- Prefer `#[repr(C)]` for structs shared across FFI boundaries.
- Validate pointer/nullability assumptions aggressively.

## Pitfalls

- ABI mismatch leading to undefined behavior.
- Passing Rust-owned memory across FFI without clear ownership contracts.
