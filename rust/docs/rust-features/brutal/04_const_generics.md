# Brutal: Const Generics

## What this feature is

Const generics let you parameterize types and functions with compile-time constant values (like array length). This enables APIs that encode structural constraints directly in types.

It is especially powerful for fixed-size buffers, numeric kernels, and protocol encodings.

## Prerequisites

- Generics and structs
- Arrays and indexing

## Self-contained example

```rust
#[derive(Debug)]
struct RingBuffer<const N: usize> {
    data: [u8; N],
    write_index: usize,
}

impl<const N: usize> RingBuffer<N> {
    fn new() -> Self {
        Self {
            data: [0; N],
            write_index: 0,
        }
    }

    fn push(&mut self, byte: u8) {
        self.data[self.write_index % N] = byte;
        self.write_index += 1;
    }
}

fn main() {
    let mut rb = RingBuffer::<4>::new();
    rb.push(10);
    rb.push(20);
    rb.push(30);
    rb.push(40);
    rb.push(50);
    println!("{:?}", rb.data); // [50, 20, 30, 40]
}
```

## Best practices

- Use const generics when size is part of the domain invariant.
- Keep API ergonomics in mind; expose sensible defaults where possible.

## Pitfalls

- Overengineering generic parameters and hurting readability.
- Assuming runtime flexibility where compile-time specialization is required.
