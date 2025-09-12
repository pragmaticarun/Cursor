use std::collections::{HashMap, HashSet};

pub fn run() {
    println!("-- collections");

    let mut v = vec![1, 2, 3];
    v.push(4);

    let mut map: HashMap<String, i32> = HashMap::new();
    map.insert("alice".to_string(), 10);
    map.entry("bob".to_string()).or_insert(0);

    let set: HashSet<_> = [1, 2, 2, 3].into_iter().collect();

    println!("v={v:?}, map={map:?}, set={set:?}");
}

