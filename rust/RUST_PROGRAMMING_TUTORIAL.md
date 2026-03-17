# Rust Programming Tutorial (Easy → Medium → Hard → Brutal)

This tutorial is organized by difficulty and designed to be practical:

- Every concept includes prerequisites, an explanation, a **self-sufficient code example**, best practices, and common pitfalls.
- Installation steps are included first.
- The examples are mirrored in `tests/rust_tutorial_examples.rs` and validated with:

```bash
cargo test --test rust_tutorial_examples
```

---

## 0) Rust Installation

### Linux / macOS / WSL

```bash
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
source "$HOME/.cargo/env"
rustc --version
cargo --version
```

### Windows (PowerShell)

```powershell
winget install Rustlang.Rustup
rustc --version
cargo --version
```

### Keep Tooling Updated

```bash
rustup update
rustup component add rustfmt clippy
```

### Create and Run a Project

```bash
cargo new hello-rust
cd hello-rust
cargo run
```

---

## 1) EASY

### 1.1 Variables, Mutability, and Shadowing

**Prerequisites:** Rust installed, terminal access.

```rust
fn main() {
    let x = 2;
    let x = x + 3; // shadowing
    let mut y = 10;
    y += x;
    println!("x={x}, y={y}");
}
```

**Best practices**
- Default to immutable (`let`) and only use `mut` when needed.
- Use shadowing to transform values while keeping names meaningful.

**Pitfalls**
- Confusing shadowing (`let x = ...`) with mutation (`x = ...`).

---

### 1.2 Scalar/Compound Types and Control Flow

**Prerequisites:** Variables and `if` statements.

```rust
fn main() {
    let numbers = [1, 2, 3, 4, 5];
    let mut sum = 0;
    for n in numbers {
        sum += n;
    }
    let label = if sum > 10 { "big" } else { "small" };
    println!("sum={sum}, label={label}");
}
```

**Best practices**
- Prefer iterator-based loops for transformations.
- Keep conditionals expression-oriented (`if` returns values).

**Pitfalls**
- Mixing integer types (`i32`, `u32`, `usize`) without explicit conversion.

---

### 1.3 Ownership, Borrowing, and Slices

**Prerequisites:** `String`, references (`&T`), functions.

```rust
fn first_word(s: &str) -> &str {
    s.split_whitespace().next().unwrap_or("")
}

fn main() {
    let original = String::from("hello rust");
    let borrowed = &original; // borrow, no move
    let cloned = original.clone(); // deep copy

    println!("borrowed={borrowed}");
    println!("first word={}", first_word(&cloned));
}
```

**Best practices**
- Use `&str` for read-only string parameters.
- Clone only when ownership transfer is truly needed.

**Pitfalls**
- Moving a value and then trying to use it again.
- Holding mutable and immutable references at the same time.

---

### 1.4 Structs, Enums, and Pattern Matching

**Prerequisites:** Basic data types and `match`.

```rust
struct User {
    name: String,
    active: bool,
}

enum Command {
    Start,
    Stop,
}

fn main() {
    let user = User {
        name: "Ari".to_string(),
        active: true,
    };
    let command = Command::Start;

    let action = match command {
        Command::Start if user.active => format!("{} started", user.name),
        Command::Start => "inactive user".to_string(),
        Command::Stop => "stopped".to_string(),
    };
    println!("{action}");
}
```

**Best practices**
- Model state with enums instead of booleans when there are multiple states.
- Use `match` guards for precise logic.

**Pitfalls**
- Using `if/else` chains where enum-based `match` is safer and clearer.

---

### 1.5 Modules, Collections, Option, and Result

**Prerequisites:** Functions and pattern matching.

```rust
mod math {
    pub fn add(a: i32, b: i32) -> i32 {
        a + b
    }
}

fn parse_port(input: &str) -> Result<u16, String> {
    let port = input.parse::<u16>().map_err(|e| e.to_string())?;
    if port == 0 {
        return Err("port must be > 0".to_string());
    }
    Ok(port)
}

fn main() {
    let mut values = vec![3, 1, 2];
    values.sort();
    println!("sum={}", math::add(20, 22));
    println!("sorted={values:?}");
    println!("port={:?}", parse_port("8080"));
}
```

**Best practices**
- Return `Result<T, E>` for fallible operations.
- Keep module APIs small and explicit with `pub`.

**Pitfalls**
- Calling `.unwrap()` in production paths instead of handling errors.

---

## 2) MEDIUM

### 2.1 Generics, Traits, and Lifetimes

**Prerequisites:** Structs, enums, references.

```rust
trait Summarize {
    fn summary(&self) -> String;
}

struct Article {
    title: String,
    author: String,
}

impl Summarize for Article {
    fn summary(&self) -> String {
        format!("{} by {}", self.title, self.author)
    }
}

fn longest<'a>(left: &'a str, right: &'a str) -> &'a str {
    if left.len() >= right.len() { left } else { right }
}

fn print_summary<T: Summarize>(item: &T) {
    println!("{}", item.summary());
}

fn main() {
    let article = Article {
        title: "Rust Patterns".to_string(),
        author: "Niko".to_string(),
    };
    print_summary(&article);
    println!("longest={}", longest("ownership", "borrow"));
}
```

**Best practices**
- Use trait bounds to express behavior requirements.
- Add explicit lifetimes only when elision rules are insufficient.

**Pitfalls**
- Treating lifetimes as runtime duration (they are compile-time relationships).

---

### 2.2 Iterators and Closures

**Prerequisites:** Vectors, functions.

```rust
fn main() {
    let values = vec![1, 2, 3, 4, 5, 6];
    let even_squares: Vec<i32> = values
        .into_iter()
        .filter(|n| n % 2 == 0)
        .map(|n| n * n)
        .collect();

    println!("{even_squares:?}");
}
```

**Best practices**
- Favor iterator chains over index-based loops when transforming data.
- Know ownership behavior: `iter()` (borrow), `iter_mut()` (mutable borrow), `into_iter()` (move).

**Pitfalls**
- Unexpected moves after calling `into_iter()`.

---

### 2.3 Custom Errors and `?` Propagation

**Prerequisites:** `Result`, enums, traits.

```rust
use std::error::Error;
use std::fmt;

#[derive(Debug)]
enum AppError {
    EmptyInput,
    Parse(std::num::ParseIntError),
}

impl fmt::Display for AppError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            AppError::EmptyInput => write!(f, "input was empty"),
            AppError::Parse(e) => write!(f, "parse error: {e}"),
        }
    }
}

impl Error for AppError {}

fn parse_nonzero(input: &str) -> Result<u32, AppError> {
    if input.trim().is_empty() {
        return Err(AppError::EmptyInput);
    }
    let value: u32 = input.parse().map_err(AppError::Parse)?;
    if value == 0 {
        return Err(AppError::EmptyInput);
    }
    Ok(value)
}

fn main() {
    println!("{:?}", parse_nonzero("7"));
}
```

**Best practices**
- Use domain-specific error enums.
- Preserve source errors when mapping.

**Pitfalls**
- Losing diagnostics by replacing all errors with a generic string too early.

---

### 2.4 Testing, Tooling, and Conditional Compilation

**Prerequisites:** Cargo basics.

```rust
#[cfg(debug_assertions)]
fn build_mode() -> &'static str { "debug" }

#[cfg(not(debug_assertions))]
fn build_mode() -> &'static str { "release" }

fn main() {
    println!("mode={}", build_mode());
}
```

**Best practices**
- Run `cargo fmt`, `cargo clippy`, and `cargo test` frequently.
- Use `#[cfg(...)]` for platform/build-mode specific code.

**Pitfalls**
- Hiding logic behind cfg flags without testing both code paths.

---

## 3) HARD

### 3.1 Smart Pointers (`Box`, `Rc`, `RefCell`)

**Prerequisites:** Ownership and borrowing.

```rust
use std::cell::RefCell;
use std::rc::Rc;

fn main() {
    let shared = Rc::new(RefCell::new(String::from("hi")));
    let a = Rc::clone(&shared);
    let b = Rc::clone(&shared);

    a.borrow_mut().push_str(" rust");
    b.borrow_mut().push('!');

    println!("{}", shared.borrow());
}
```

**Best practices**
- Use `Rc<T>` for shared ownership in single-threaded contexts.
- Use `RefCell<T>` only when compile-time borrowing is too restrictive.

**Pitfalls**
- Runtime panics from violating `RefCell` borrow rules.
- Using `Rc<T>` across threads (use `Arc<T>` instead).

---

### 3.2 Concurrency with Threads, Channels, and `Arc<Mutex<T>>`

**Prerequisites:** Closures (`move`) and ownership transfer.

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
            *counter.lock().unwrap() += 1;
            tx.send(id * 2).unwrap();
        }));
    }
    drop(tx);

    let mut received: Vec<_> = rx.iter().collect();
    received.sort_unstable();
    for h in handles {
        h.join().unwrap();
    }

    println!("received={received:?}");
    println!("counter={}", *counter.lock().unwrap());
}
```

**Best practices**
- Keep lock scope as small as possible.
- Prefer message passing for ownership transfer across threads.

**Pitfalls**
- Holding mutex guards too long.
- Deadlocks from lock ordering issues.

---

### 3.3 Async/Await Fundamentals (Self-Contained Executor)

**Prerequisites:** Futures and trait basics.

```rust
use std::future::Future;
use std::pin::Pin;
use std::task::{Context, Poll, RawWaker, RawWakerVTable, Waker};
use std::thread;

fn noop_waker() -> Waker {
    fn clone(_: *const ()) -> RawWaker { RawWaker::new(std::ptr::null(), &VTABLE) }
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
            Poll::Ready(v) => return v,
            Poll::Pending => thread::yield_now(),
        }
    }
}

async fn async_add(left: i32, right: i32) -> i32 { left + right }

fn main() {
    let answer = block_on(async_add(20, 22));
    println!("{answer}");
}
```

**Best practices**
- In real services, use mature runtimes (`tokio`, `async-std`) rather than a toy executor.

**Pitfalls**
- Blocking threads inside async code.

---

### 3.4 Macros and Trait Objects

**Prerequisites:** Traits, generics, and pattern matching.

```rust
macro_rules! pair_vec {
    ($($key:expr => $value:expr),* $(,)?) => {{
        let mut tmp = Vec::new();
        $( tmp.push(($key, $value)); )*
        tmp
    }};
}

trait Shape {
    fn area(&self) -> f64;
}

struct Rectangle {
    width: f64,
    height: f64,
}

impl Shape for Rectangle {
    fn area(&self) -> f64 {
        self.width * self.height
    }
}

fn main() {
    let pairs = pair_vec!("rust" => 1, "cargo" => 2);
    let shapes: Vec<Box<dyn Shape>> = vec![Box::new(Rectangle { width: 3.0, height: 4.0 })];
    println!("{pairs:?} area={}", shapes[0].area());
}
```

**Best practices**
- Use trait objects (`dyn Trait`) for runtime polymorphism.
- Keep macro interfaces small and predictable.

**Pitfalls**
- Overusing macros where functions/generics are clearer.

---

## 4) BRUTAL

### 4.1 Unsafe Rust and FFI

**Prerequisites:** Pointers, linking basics, ownership model.

```rust
unsafe extern "C" {
    fn abs(input: i32) -> i32;
}

fn main() {
    let mut value = 41;
    let ptr = &mut value as *mut i32;
    unsafe { *ptr += 1; }
    println!("value={value}");

    let absolute = unsafe { abs(-42) };
    println!("abs={absolute}");
}
```

**Best practices**
- Keep `unsafe` blocks tiny and heavily justified.
- Wrap unsafe internals behind safe APIs.

**Pitfalls**
- Dereferencing invalid pointers.
- Violating aliasing or lifetime assumptions.

---

### 4.2 Atomics and Memory Ordering

**Prerequisites:** Threading and shared state.

```rust
use std::sync::atomic::{AtomicUsize, Ordering};
use std::sync::Arc;
use std::thread;

fn main() {
    let counter = Arc::new(AtomicUsize::new(0));
    let mut handles = Vec::new();

    for _ in 0..8 {
        let counter = Arc::clone(&counter);
        handles.push(thread::spawn(move || {
            counter.fetch_add(1, Ordering::SeqCst);
        }));
    }

    for h in handles {
        h.join().unwrap();
    }

    println!("counter={}", counter.load(Ordering::SeqCst));
}
```

**Best practices**
- Start with `SeqCst`; relax orderings only with clear proof/benchmark motivation.

**Pitfalls**
- Incorrect memory ordering assumptions that produce rare race bugs.

---

### 4.3 Const Generics and Compile-Time Configuration

**Prerequisites:** Generics and arrays.

```rust
#[derive(Debug)]
struct RingBuffer<const N: usize> {
    data: [u8; N],
    write_index: usize,
}

impl<const N: usize> RingBuffer<N> {
    fn new() -> Self {
        Self { data: [0; N], write_index: 0 }
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

**Best practices**
- Use const generics to encode fixed-size constraints in types.

**Pitfalls**
- Over-parameterizing APIs and making error messages hard to read.

---

## 5) Language Feature Coverage Checklist

This tutorial covers:

- Ownership, borrowing, references, slices
- Primitive and compound types
- Control flow and pattern matching
- Structs, enums, methods, modules, visibility
- Collections (`Vec`, `String`) and iterators
- `Option`, `Result`, custom errors, `?`
- Generics, traits, trait bounds, trait objects
- Lifetimes
- Closures and iterator adapters
- Testing/tooling (`cargo test`, `fmt`, `clippy`)
- Conditional compilation (`cfg`)
- Smart pointers (`Box`, `Rc`, `RefCell`, `Arc`, `Mutex`)
- Concurrency (threads, channels)
- Async/await fundamentals
- Declarative macros (`macro_rules!`)
- Unsafe Rust basics
- FFI (`extern "C"`)
- Atomics and memory ordering
- Const generics

---

## 6) Recommended Best Practices (Global)

1. Prefer safe Rust; isolate `unsafe`.
2. Keep ownership simple and explicit.
3. Design small types with clear invariants.
4. Return `Result` instead of panicking in library code.
5. Run `cargo fmt`, `cargo clippy`, and `cargo test` in CI.
6. Benchmark before optimizing (`cargo bench`, `cargo flamegraph` where applicable).
7. Document assumptions and edge cases in API docs.

---

## 7) Common Pitfalls (Global)

1. Overusing `.unwrap()` and `.expect()` in non-test code.
2. Cloning too much to bypass ownership issues.
3. Choosing `Rc<RefCell<T>>` when plain borrowing would work.
4. Holding mutex guards across expensive operations.
5. Prematurely using advanced atomics/unsafe without necessity.
6. Confusing trait-object dynamic dispatch with generic static dispatch.

---

## 8) How to Validate the Examples

From this repository root:

```bash
cargo test --test rust_tutorial_examples
```

That command compiles and runs the tutorial-backed examples end-to-end.
