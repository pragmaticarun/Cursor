# Medium: Generics, Traits, and Lifetimes

## What this feature is

Generics let you write reusable code over multiple types without runtime overhead. Traits define shared behavior contracts. Lifetimes describe how references relate, ensuring no dangling references at compile time.

Together, these features power Rust’s zero-cost abstractions: high-level APIs with low-level performance.

## Prerequisites

- Ownership and borrowing
- Structs and functions

## Self-contained example

```rust
trait Summarize {
    fn summary(&self) -> String;
}

struct Article {
    title: String,
    author: String,
}

impl Summarize for Article {
    fn summary(&self) -> String {
        format!("{} by {}", self.title, self.author)
    }
}

fn print_summary<T: Summarize>(item: &T) {
    println!("{}", item.summary());
}

fn longest<'a>(left: &'a str, right: &'a str) -> &'a str {
    if left.len() >= right.len() { left } else { right }
}

fn main() {
    let article = Article {
        title: "Rust Patterns".to_string(),
        author: "Niko".to_string(),
    };
    print_summary(&article);
    println!("longest={}", longest("ownership", "borrow"));
}
```

## Best practices

- Use trait bounds to define capability requirements.
- Keep lifetime annotations minimal and meaningful.
- Prefer generic APIs where static dispatch is beneficial.

## Pitfalls

- Treating lifetimes as runtime duration.
- Over-constraining generics, hurting API ergonomics.
