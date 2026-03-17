# Hard: Declarative Macros (`macro_rules!`)

## What this feature is

Macros in Rust generate code at compile time. Declarative macros (`macro_rules!`) match token patterns and expand them into Rust code. They are ideal when repeated structure cannot be expressed ergonomically with functions or generics.

## Prerequisites

- Functions and pattern syntax
- Understanding of expression contexts

## Self-contained example

```rust
macro_rules! pair_vec {
    ($($k:expr => $v:expr),* $(,)?) => {{
        let mut out = Vec::new();
        $(
            out.push(($k, $v));
        )*
        out
    }};
}

fn main() {
    let pairs = pair_vec!("rust" => 1, "cargo" => 2, "clippy" => 3);
    println!("{pairs:?}");
}
```

## Best practices

- Keep macro input grammar minimal and obvious.
- Emit readable expansions and good error messages when possible.
- Prefer functions/generics unless macro power is truly needed.

## Pitfalls

- Overusing macros and making code hard to trace.
- Hidden side effects inside macro expansions.
