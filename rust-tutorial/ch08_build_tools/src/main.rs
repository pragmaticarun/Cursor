use clap::Parser;

#[derive(Parser, Debug)]
#[command(name = "wc-mini", about = "Count lines/words/chars of a file")]
struct Args {
    /// Path to input file
    path: String,

    /// Count bytes instead of chars
    #[arg(long)]
    bytes: bool,
}

fn main() {
    println!("== Chapter 08: Build Tools & Env ==");
    let args = Args::parse();
    let content = std::fs::read(args.path).expect("read");
    if args.bytes {
        println!("bytes={}", content.len());
    } else {
        let s = String::from_utf8_lossy(&content);
        let lines = s.lines().count();
        let words = s.split_whitespace().count();
        let chars = s.chars().count();
        println!("lines={lines} words={words} chars={chars}");
    }
}

