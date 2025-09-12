pub fn run() {
    println!("-- ownership");

    let s = String::from("hello");
    let s2 = s; // move occurs
    // println!("{s}"); // would not compile
    println!("moved to s2={s2}");

    let a = 10; // Copy type
    let b = a; // copy, a still usable
    println!("a={a}, b={b}");
}

