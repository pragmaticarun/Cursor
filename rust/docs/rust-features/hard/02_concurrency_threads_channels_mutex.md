# Hard: Concurrency with Threads, Channels, and `Arc<Mutex<T>>`

## What this feature is

Rust concurrency is based on ownership transfer, borrowing rules, and thread-safe primitives:

- `thread::spawn` to run work in parallel
- channels (`mpsc`) for message passing
- `Arc<Mutex<T>>` for shared mutable state across threads

The compiler prevents data races by construction.

## Prerequisites

- Closures (`move`)
- Ownership transfer semantics

## Self-contained example

```rust
use std::sync::{mpsc, Arc, Mutex};
use std::thread;

fn main() {
    let (tx, rx) = mpsc::channel();
    let counter = Arc::new(Mutex::new(0usize));
    let mut handles = Vec::new();

    for id in 0..4 {
        let tx = tx.clone();
        let counter = Arc::clone(&counter);
        handles.push(thread::spawn(move || {
            *counter.lock().expect("mutex poisoned") += 1;
            tx.send(id * 2).expect("send failed");
        }));
    }
    drop(tx);

    let mut received: Vec<_> = rx.iter().collect();
    received.sort_unstable();

    for h in handles {
        h.join().expect("thread join failed");
    }

    println!("received={received:?}");
    println!("counter={}", *counter.lock().expect("mutex poisoned"));
}
```

## Best practices

- Prefer message passing to reduce lock complexity.
- Keep lock hold time as short as possible.
- Handle poison errors intentionally in production systems.

## Pitfalls

- Deadlocks from inconsistent lock order.
- Holding locks while doing expensive I/O.
