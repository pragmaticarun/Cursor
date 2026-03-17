# Hard: Trait Objects and Dynamic Dispatch

## What this feature is

Trait objects (`dyn Trait`) let you store values of different concrete types behind a common interface. Calls are dynamically dispatched at runtime, which is useful when type sets are open-ended or plugin-like.

This contrasts with generics, which use static dispatch and monomorphization at compile time.

## Prerequisites

- Traits and impl blocks
- Heap allocation with `Box`

## Self-contained example

```rust
trait Shape {
    fn area(&self) -> f64;
}

struct Rectangle {
    width: f64,
    height: f64,
}

struct Circle {
    radius: f64,
}

impl Shape for Rectangle {
    fn area(&self) -> f64 {
        self.width * self.height
    }
}

impl Shape for Circle {
    fn area(&self) -> f64 {
        std::f64::consts::PI * self.radius * self.radius
    }
}

fn main() {
    let shapes: Vec<Box<dyn Shape>> = vec![
        Box::new(Rectangle { width: 3.0, height: 4.0 }),
        Box::new(Circle { radius: 2.0 }),
    ];

    for (idx, s) in shapes.iter().enumerate() {
        println!("shape#{idx} area={:.2}", s.area());
    }
}
```

## Best practices

- Use generics when the concrete type is known at compile time.
- Use trait objects for heterogeneous collections and plugin boundaries.

## Pitfalls

- Expecting trait objects to support methods requiring `Self: Sized`.
- Ignoring dynamic dispatch overhead in ultra-hot paths.
