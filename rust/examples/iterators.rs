use std::collections::HashMap;

fn main() {
    let v = vec![1, 2, 3, 4, 5];
    let squares: Vec<_> = v.iter().map(|x| x * x).collect();
    println!("{:?}", squares);

    let mut m = HashMap::new();
    for w in ["hello", "hello", "world"] {
        *m.entry(w).or_insert(0) += 1;
    }
    println!("{:?}", m);
}