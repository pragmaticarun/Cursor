pub fn run() {
    println!("-- ownership & borrowing");
    let s = String::from("abc");
    takes_ownership(s);
    let n = 5;
    makes_copy(n);
    let mut t = String::from("abc");
    borrow_mut(&mut t);
}

fn takes_ownership(s: String) { println!("owned: {s}"); }
fn makes_copy(n: i32) { println!("copy: {n}"); }
fn borrow_mut(s: &mut String) { s.push('!'); println!("mut: {s}"); }

