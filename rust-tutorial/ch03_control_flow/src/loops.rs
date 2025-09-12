pub fn run() {
    println!("-- loops");

    for x in (1..=5).map(|x| x * 2) {
        if x == 6 { continue; }
        println!("for x={x}");
    }

    let mut maybe = Some(3);
    while let Some(v) = maybe {
        println!("while v={v}");
        maybe = if v > 0 { Some(v - 1) } else { None };
    }

    let mut count = 0;
    let res = loop {
        count += 1;
        if count == 3 { break count * 10; }
    };
    println!("loop result={res}");
}

