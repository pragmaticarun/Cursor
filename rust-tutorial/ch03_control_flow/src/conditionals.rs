pub fn run() {
    println!("-- conditionals");
    let n = 7;
    let label = if n == 0 { "zero" } else if n % 2 == 0 { "even" } else { "odd" };
    println!("n={n}, label={label}");
}

