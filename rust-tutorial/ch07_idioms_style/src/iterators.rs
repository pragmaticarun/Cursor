pub fn run() {
    println!("-- iterator adapters");
    let data = vec![1, 2, 3, 4];
    let sum: i32 = data.iter().filter(|x| **x % 2 == 0).map(|x| x * 10).sum();
    println!("sum={sum}");
}

