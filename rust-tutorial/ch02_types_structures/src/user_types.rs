#[derive(Debug, Clone)]
struct User {
    id: u64,
    name: String,
}

#[derive(Debug)]
enum AuthState {
    Guest,
    LoggedIn(User),
}

trait Greeter {
    fn greet(&self) -> String;
}

impl Greeter for User {
    fn greet(&self) -> String { format!("Hello, {}!", self.name) }
}

pub fn run() {
    println!("-- user-defined types");

    let user = User { id: 1, name: "Alice".into() };
    let state = AuthState::LoggedIn(user.clone());

    match state {
        AuthState::Guest => println!("guest"),
        AuthState::LoggedIn(u) => println!("{} (id={})", u.greet(), u.id),
    }
}

