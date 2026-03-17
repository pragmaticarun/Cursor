# Hard: Smart Pointers (`Box`, `Rc`, `RefCell`, `Arc`, `Mutex`)

## What this feature is

Smart pointers extend raw ownership with additional semantics:

- `Box<T>`: heap allocation with single ownership
- `Rc<T>`: reference-counted shared ownership (single-threaded)
- `RefCell<T>`: runtime-checked interior mutability (single-threaded)
- `Arc<T>`: atomic reference counting (thread-safe shared ownership)
- `Mutex<T>`: interior mutability with mutual exclusion across threads

These types are essential when plain borrowing cannot model the ownership graph you need.

## Prerequisites

- Ownership and borrowing
- Structs and methods

## Self-contained example

```rust
use std::cell::RefCell;
use std::rc::Rc;

fn main() {
    let shared = Rc::new(RefCell::new(String::from("hi")));
    let a = Rc::clone(&shared);
    let b = Rc::clone(&shared);

    a.borrow_mut().push_str(" rust");
    b.borrow_mut().push('!');

    println!("value={}", shared.borrow());
    println!("strong_count={}", Rc::strong_count(&shared));
}
```

## Best practices

- Start with plain references and upgrade only when necessary.
- Keep `RefCell` borrow scopes narrow to avoid runtime panics.
- Use `Arc<T>` instead of `Rc<T>` in multi-threaded contexts.

## Pitfalls

- Choosing `Rc<RefCell<T>>` as a default architecture pattern.
- Forgetting that `RefCell` enforces borrowing at runtime, not compile time.
