pub fn run() {
    println!("-- scalars & compounds");

    // Scalars
    let i: i32 = -42;
    let u: u64 = 42;
    let f: f64 = 3.14;
    let b: bool = true;
    let c: char = '🦀';

    // Tuples and arrays
    let t: (i32, bool) = (7, false);
    let arr: [u8; 3] = [1, 2, 3];
    let slice: &[u8] = &arr[1..];

    println!("i={i}, u={u}, f={f}, b={b}, c={c}, t0={}, arr={arr:?}, slice={slice:?}", t.0);
}

