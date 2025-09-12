mod basics;
mod ownership;
mod borrowing;

fn main() {
    println!("== Chapter 01: Syntax & Semantics ==");
    basics::run();
    ownership::run();
    borrowing::run();
}

