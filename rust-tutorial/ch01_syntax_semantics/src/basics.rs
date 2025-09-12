pub fn run() {
    println!("-- basics");

    // let bindings: immutable by default, shadowing, mutability
    let x = 5;
    let x = x + 1; // shadowing
    let mut y = 0; // mutable
    y += 1;

    // expressions
    let z = if x > 5 { 10 } else { 0 };

    // block expression
    let w = {
        let a = 2;
        a * 3
    };

    println!("x={x}, y={y}, z={z}, w={w}");
}

