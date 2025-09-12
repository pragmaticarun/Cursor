mod inner {
    pub mod nested {
        pub fn hello() -> &'static str { "hello from nested" }
    }

    pub fn hidden() -> &'static str { "hidden" }
}

pub fn run() {
    use inner::nested::hello;
    println!("{}", hello());
    println!("{}", inner::hidden());
}

