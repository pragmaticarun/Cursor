# Brutal: Atomics and Memory Ordering

## What this feature is

Atomics provide lock-free synchronization primitives for shared data. Memory ordering controls visibility and reordering guarantees between threads. Correct ordering is central to writing race-free lock-free code.

Common orderings:

- `SeqCst`: strongest and easiest to reason about
- `Acquire`/`Release`: paired synchronization for reads/writes
- `Relaxed`: atomicity only, minimal ordering guarantees

## Prerequisites

- Threading basics
- Shared-state concurrency concepts

## Self-contained example

```rust
use std::sync::atomic::{AtomicUsize, Ordering};
use std::sync::Arc;
use std::thread;

fn main() {
    let counter = Arc::new(AtomicUsize::new(0));
    let mut handles = Vec::new();

    for _ in 0..8 {
        let c = Arc::clone(&counter);
        handles.push(thread::spawn(move || {
            c.fetch_add(1, Ordering::SeqCst);
        }));
    }

    for h in handles {
        h.join().expect("thread join failed");
    }

    println!("counter={}", counter.load(Ordering::SeqCst));
}
```

## Best practices

- Start with `SeqCst`; optimize ordering only with evidence.
- Add comments explaining synchronization intent.
- Test under stress and use tools like `loom` for concurrency models.

## Pitfalls

- Using `Relaxed` where happens-before guarantees are required.
- Assuming atomics alone make compound operations logically safe.
