fn len_shared(s: &String) -> usize { s.len() }
fn push_exclusive(s: &mut String) { s.push_str(" world"); }

pub fn run() {
    println!("-- borrowing");

    let s = String::from("hello");
    let l = len_shared(&s); // shared borrow
    println!("len={l}, s={s}");

    let mut t = String::from("hello");
    push_exclusive(&mut t); // exclusive borrow
    println!("t={t}");
}

