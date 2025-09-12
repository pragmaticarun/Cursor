fn longest<'a>(a: &'a str, b: &'a str) -> &'a str {
    if a.len() >= b.len() { a } else { b }
}

pub fn run() {
    println!("-- lifetimes");
    let a = String::from("hello");
    let b = String::from("world!!!");
    let r = longest(&a, &b);
    println!("longest={r}");
}

