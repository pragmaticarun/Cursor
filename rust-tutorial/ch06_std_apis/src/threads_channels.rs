use std::sync::mpsc;
use std::thread;

pub fn run() {
    println!("-- threads & channels");
    let (tx, rx) = mpsc::channel();
    let handle = thread::spawn(move || {
        for i in 0..5 { tx.send(i).unwrap(); }
    });
    for v in rx { println!("got {v}"); }
    handle.join().unwrap();
}

