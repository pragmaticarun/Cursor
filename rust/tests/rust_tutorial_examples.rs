use std::cell::RefCell;
use std::error::Error;
use std::fmt;
use std::future::Future;
use std::pin::Pin;
use std::rc::Rc;
use std::sync::atomic::{AtomicUsize, Ordering};
use std::sync::{mpsc, Arc, Mutex};
use std::task::{Context, Poll, RawWaker, RawWakerVTable, Waker};
use std::thread;

unsafe extern "C" {
    fn abs(input: i32) -> i32;
}

#[test]
fn easy_variables_and_shadowing() {
    let x = 2;
    let x = x + 3;
    let mut y = 10;
    y += x;

    assert_eq!(x, 5);
    assert_eq!(y, 15);
}

#[test]
fn easy_types_and_control_flow() {
    let numbers = [1, 2, 3, 4, 5];
    let mut sum = 0;
    for n in numbers {
        sum += n;
    }

    let label = if sum > 10 { "big" } else { "small" };
    assert_eq!(sum, 15);
    assert_eq!(label, "big");
}

#[test]
fn easy_ownership_borrowing_and_slices() {
    fn first_word(s: &str) -> &str {
        s.split_whitespace().next().unwrap_or("")
    }

    let original = String::from("hello rust");
    let borrowed = &original;
    let cloned = original.clone();

    assert_eq!(borrowed, "hello rust");
    assert_eq!(first_word(&cloned), "hello");
}

#[test]
fn easy_structs_enums_and_match() {
    struct User {
        name: String,
        active: bool,
    }

    enum Command {
        Start,
        Stop,
    }

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

    assert_eq!(action, "Ari started");

    let stop = Command::Stop;
    let action = match stop {
        Command::Start => "started".to_string(),
        Command::Stop => "stopped".to_string(),
    };
    assert_eq!(action, "stopped");
}

#[test]
fn easy_modules_collections_and_option_result() {
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

    let mut values = vec![3, 1, 2];
    values.sort();

    assert_eq!(math::add(20, 22), 42);
    assert_eq!(values, vec![1, 2, 3]);
    assert_eq!(parse_port("8080").ok(), Some(8080));
    assert!(parse_port("0").is_err());
    assert!(parse_port("not-a-number").is_err());
}

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

fn print_summary<T: Summarize>(item: &T) -> String {
    item.summary()
}

#[test]
fn medium_generics_traits_and_lifetimes() {
    fn longest<'a>(left: &'a str, right: &'a str) -> &'a str {
        if left.len() >= right.len() {
            left
        } else {
            right
        }
    }

    let article = Article {
        title: "Rust Patterns".to_string(),
        author: "Niko".to_string(),
    };

    assert_eq!(print_summary(&article), "Rust Patterns by Niko");
    assert_eq!(longest("ownership", "borrow"), "ownership");
}

#[test]
fn medium_iterators_and_closures() {
    let values = vec![1, 2, 3, 4, 5, 6];
    let even_squares: Vec<i32> = values
        .into_iter()
        .filter(|n| n % 2 == 0)
        .map(|n| n * n)
        .collect();

    assert_eq!(even_squares, vec![4, 16, 36]);
}

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

#[test]
fn medium_custom_errors_and_question_mark() {
    assert_eq!(parse_nonzero("7").ok(), Some(7));
    assert!(parse_nonzero("0").is_err());
    assert!(parse_nonzero("x").is_err());
}

#[test]
fn hard_smart_pointers_rc_and_refcell() {
    let shared = Rc::new(RefCell::new(String::from("hi")));
    let a = Rc::clone(&shared);
    let b = Rc::clone(&shared);

    a.borrow_mut().push_str(" rust");
    b.borrow_mut().push('!');

    assert_eq!(shared.borrow().as_str(), "hi rust!");
    assert_eq!(Rc::strong_count(&shared), 3);
}

#[test]
fn hard_threads_channels_and_mutex() {
    let (tx, rx) = mpsc::channel();
    let workers = 4;
    let counter = Arc::new(Mutex::new(0usize));
    let mut handles = Vec::new();

    for id in 0..workers {
        let tx = tx.clone();
        let counter = Arc::clone(&counter);
        handles.push(thread::spawn(move || {
            {
                let mut guard = counter.lock().expect("mutex poisoned");
                *guard += 1;
            }
            tx.send(id * 2).expect("send failed");
        }));
    }
    drop(tx);

    let mut received: Vec<_> = rx.iter().collect();
    received.sort_unstable();

    for handle in handles {
        handle.join().expect("thread join failed");
    }

    assert_eq!(received, vec![0, 2, 4, 6]);
    assert_eq!(*counter.lock().expect("mutex poisoned"), workers);
}

fn noop_waker() -> Waker {
    fn clone(_: *const ()) -> RawWaker {
        RawWaker::new(std::ptr::null(), &VTABLE)
    }
    fn noop(_: *const ()) {}
    static VTABLE: RawWakerVTable = RawWakerVTable::new(clone, noop, noop, noop);
    let raw = RawWaker::new(std::ptr::null(), &VTABLE);
    unsafe { Waker::from_raw(raw) }
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

async fn async_add(left: i32, right: i32) -> i32 {
    left + right
}

#[test]
fn hard_async_await_basics() {
    let result = block_on(async_add(20, 22));
    assert_eq!(result, 42);
}

macro_rules! pair_vec {
    ($($key:expr => $value:expr),* $(,)?) => {{
        let mut tmp = Vec::new();
        $(
            tmp.push(($key, $value));
        )*
        tmp
    }};
}

#[test]
fn hard_declarative_macros() {
    let pairs = pair_vec!("rust" => 1, "cargo" => 2);
    assert_eq!(pairs, vec![("rust", 1), ("cargo", 2)]);
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

#[test]
fn hard_trait_objects() {
    let shapes: Vec<Box<dyn Shape>> = vec![Box::new(Rectangle {
        width: 3.0,
        height: 4.0,
    })];
    assert_eq!(shapes[0].area(), 12.0);
}

#[test]
fn brutal_unsafe_and_ffi() {
    let mut value = 41;
    let ptr = &mut value as *mut i32;
    unsafe {
        *ptr += 1;
    }
    assert_eq!(value, 42);

    let absolute = unsafe { abs(-42) };
    assert_eq!(absolute, 42);
}

#[test]
fn brutal_atomics_and_memory_ordering() {
    let counter = Arc::new(AtomicUsize::new(0));
    let mut handles = Vec::new();

    for _ in 0..8 {
        let counter = Arc::clone(&counter);
        handles.push(thread::spawn(move || {
            counter.fetch_add(1, Ordering::SeqCst);
        }));
    }

    for handle in handles {
        handle.join().expect("thread join failed");
    }

    assert_eq!(counter.load(Ordering::SeqCst), 8);
}

#[derive(Debug, PartialEq)]
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

#[test]
fn brutal_const_generics() {
    let mut rb = RingBuffer::<4>::new();
    rb.push(10);
    rb.push(20);
    rb.push(30);
    rb.push(40);
    rb.push(50);

    assert_eq!(rb.data, [50, 20, 30, 40]);
}
