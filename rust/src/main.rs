use std::collections::HashMap;
use std::fs;

fn word_count(s: &str) -> HashMap<String, usize> {
    let mut m = HashMap::new();
    for w in s.split_whitespace() {
        *m.entry(w.to_lowercase()).or_insert(0) += 1;
    }
    m
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    println!("=== Rust demo ===");

    let text = "Hello hello world";
    println!("{:?}", word_count(text));

    let here = std::env::current_dir()?;
    for (i, entry) in fs::read_dir(here)?.enumerate() {
        if i >= 3 { break; }
        println!("- {:?}", entry?.path());
    }

    Ok(())
}