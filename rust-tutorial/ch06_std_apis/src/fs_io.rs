use std::{fs, io::{self, Read}, path::PathBuf};

pub fn run() {
    println!("-- fs & io");
    let mut path = PathBuf::from(env!("CARGO_MANIFEST_DIR"));
    path.push("demo.txt");
    let _ = fs::write(&path, b"hello std io\n");
    let mut file = fs::File::open(&path).expect("open");
    let mut buf = String::new();
    file.read_to_string(&mut buf).expect("read");
    println!("read: {buf}");
}

