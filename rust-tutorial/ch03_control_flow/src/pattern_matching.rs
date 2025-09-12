pub fn run() {
    println!("-- match & if let");

    let n = 6;
    let kind = match n {
        0 => "zero",
        1..=3 => "small",
        x if x % 2 == 0 => "even",
        _ => "other",
    };
    println!("kind={kind}");

    let maybe = Some("value");
    if let Some(v) = maybe { println!("got {v}"); }
}

