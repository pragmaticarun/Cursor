#[derive(Debug)]
enum Payment {
    Cash { amount: u64 },
    Card { last4: String },
    Crypto { ticker: String },
}

pub fn run() {
    println!("-- enums for validity");
    let p = Payment::Card { last4: "1234".into() };
    match p {
        Payment::Cash { amount } => println!("cash {amount}"),
        Payment::Card { last4 } => println!("card ****{last4}"),
        Payment::Crypto { ticker } => println!("crypto {ticker}"),
    }
}

