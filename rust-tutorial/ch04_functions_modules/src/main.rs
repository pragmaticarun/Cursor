mod math;
mod modules;

fn main() {
    println!("== Chapter 04: Functions & Modules ==");
    println!("add(2,3)={}", math::add(2, 3));
    println!("sum=[1,2,3] -> {}", math::sum([1, 2, 3]));
    modules::run();
}

