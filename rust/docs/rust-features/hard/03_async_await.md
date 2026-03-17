# Hard: Async/Await Fundamentals

## What this feature is

`async` functions in Rust return futures. A future is a state machine that makes progress when polled by an executor. `await` suspends logically (without blocking the OS thread) until a future is ready.

This model enables high-concurrency I/O systems with predictable overhead.

## Prerequisites

- Traits and generics
- Basic concurrency concepts

## Self-contained example

```rust
use std::future::Future;
use std::pin::Pin;
use std::task::{Context, Poll, RawWaker, RawWakerVTable, Waker};
use std::thread;

fn noop_waker() -> Waker {
    fn clone(_: *const ()) -> RawWaker {
        RawWaker::new(std::ptr::null(), &VTABLE)
    }
    fn noop(_: *const ()) {}
    static VTABLE: RawWakerVTable = RawWakerVTable::new(clone, noop, noop, noop);
    unsafe { Waker::from_raw(RawWaker::new(std::ptr::null(), &VTABLE)) }
}

fn block_on<F: Future>(future: F) -> F::Output {
    let waker = noop_waker();
    let mut cx = Context::from_waker(&waker);
    let mut future = Box::pin(future);
    loop {
        match Future::poll(Pin::as_mut(&mut future), &mut cx) {
            Poll::Ready(value) => return value,
            Poll::Pending => thread::yield_now(),
        }
    }
}

async fn async_add(a: i32, b: i32) -> i32 {
    a + b
}

fn main() {
    let value = block_on(async_add(20, 22));
    println!("value={value}");
}
```

## Best practices

- Use battle-tested executors (`tokio`) for real applications.
- Keep async code non-blocking; move CPU-heavy work to dedicated threads.

## Pitfalls

- Blocking inside async tasks, which harms scheduler fairness.
- Confusing concurrency (many tasks) with parallelism (many cores).
