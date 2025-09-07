use std::collections::HashMap;

pub fn word_count(s: &str) -> HashMap<String, usize> {
    let mut m = HashMap::new();
    for w in s.split_whitespace() {
        *m.entry(w.to_lowercase()).or_insert(0) += 1;
    }
    m
}