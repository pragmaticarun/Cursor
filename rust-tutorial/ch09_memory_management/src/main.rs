mod ownership_borrow;
mod smart_pointers;
mod lifetimes_demo;

fn main() {
    println!("== Chapter 09: Memory Management ==");
    ownership_borrow::run();
    smart_pointers::run();
    lifetimes_demo::run();
}

