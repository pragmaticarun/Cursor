# Easy: Types and Control Flow

## What this feature is

Rust has strong static typing with no implicit numeric conversions. You get compile-time guarantees that significantly reduce runtime surprises.

Control flow (`if`, `match`, `loop`, `while`, `for`) is expression-oriented. For example, `if` returns a value, so branch logic can be concise and type-checked.

## Prerequisites

- Variables and expressions
- Basic arithmetic

## Self-contained example

```rust
fn main() {
    let numbers: [i32; 5] = [1, 2, 3, 4, 5];
    let mut sum = 0;

    for n in numbers {
        sum += n;
    }

    let label = if sum > 10 { "big" } else { "small" };

    let parity = match sum % 2 {
        0 => "even",
        _ => "odd",
    };

    println!("sum={sum}, label={label}, parity={parity}");
}
```

## Best practices

- Use explicit types when intent is not obvious.
- Prefer `match` for exhaustive branching.
- Treat warnings about type conversions seriously.

## Pitfalls

- Mixing numeric types (`i32`, `u32`, `usize`) without explicit conversion.
- Forgetting that both `if` branches must resolve to the same type.
