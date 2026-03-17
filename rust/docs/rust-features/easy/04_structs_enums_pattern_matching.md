# Easy: Structs, Enums, and Pattern Matching

## What this feature is

Structs model data with named fields. Enums model a value that can be one of several variants, each potentially carrying data. Together, they create robust domain modeling with compile-time exhaustiveness via `match`.

## Prerequisites

- Functions and basic types
- Control flow (`if`, `match`)

## Self-contained example

```rust
#[derive(Debug)]
struct User {
    name: String,
    active: bool,
}

#[derive(Debug)]
enum Command {
    Start { retries: u8 },
    Stop,
}

fn handle(user: &User, cmd: Command) -> String {
    match cmd {
        Command::Start { retries } if user.active => {
            format!("{} started with retries={retries}", user.name)
        }
        Command::Start { .. } => "inactive user cannot start".to_string(),
        Command::Stop => "stopped".to_string(),
    }
}

fn main() {
    let user = User {
        name: "Ari".to_string(),
        active: true,
    };
    println!("{}", handle(&user, Command::Start { retries: 3 }));
    println!("{}", handle(&user, Command::Stop));
}
```

## Best practices

- Use enums to represent finite states/events explicitly.
- Let `match` force you to handle all cases.
- Derive `Debug` for fast diagnostics.

## Pitfalls

- Using booleans where an enum would be safer and clearer.
- Adding wildcard (`_`) arms too early and hiding future missing cases.
