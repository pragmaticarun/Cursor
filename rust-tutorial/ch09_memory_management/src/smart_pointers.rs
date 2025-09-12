use std::rc::Rc;
use std::sync::{Arc, Mutex};

pub fn run() {
    println!("-- smart pointers");
    let a = Rc::new(vec![1, 2, 3]);
    let b = a.clone();
    println!("rc counts: a={}, b={}", Rc::strong_count(&a), Rc::strong_count(&b));

    let shared = Arc::new(Mutex::new(0));
    let s2 = Arc::clone(&shared);
    {
        let mut guard = s2.lock().unwrap();
        *guard += 1;
    }
    println!("shared={:?}", shared.lock().unwrap());
}

