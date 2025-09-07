use tutorial::word_count;

#[test]
fn counts_words() {
    let c = word_count("Hello hello world");
    assert_eq!(c.get("hello"), Some(&2usize));
    assert_eq!(c.get("world"), Some(&1usize));
}