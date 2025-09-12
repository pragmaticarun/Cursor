use clap::Parser;
use std::io::{self, Write};

#[derive(Parser)]
#[command(name = "calculator")]
#[command(about = "A simple command-line calculator")]
struct Cli {
    /// Operation to perform (add, subtract, multiply, divide)
    #[arg(short, long)]
    operation: String,
    
    /// First number
    #[arg(short, long)]
    first: f64,
    
    /// Second number
    #[arg(short, long)]
    second: f64,
}

#[derive(Debug)]
enum CalculatorError {
    DivisionByZero,
    InvalidOperation,
}

impl std::fmt::Display for CalculatorError {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        match self {
            CalculatorError::DivisionByZero => write!(f, "Error: Division by zero"),
            CalculatorError::InvalidOperation => write!(f, "Error: Invalid operation"),
        }
    }
}

fn calculate(operation: &str, first: f64, second: f64) -> Result<f64, CalculatorError> {
    match operation.to_lowercase().as_str() {
        "add" | "+" => Ok(first + second),
        "subtract" | "sub" | "-" => Ok(first - second),
        "multiply" | "mul" | "*" => Ok(first * second),
        "divide" | "div" | "/" => {
            if second == 0.0 {
                Err(CalculatorError::DivisionByZero)
            } else {
                Ok(first / second)
            }
        }
        _ => Err(CalculatorError::InvalidOperation),
    }
}

fn interactive_mode() {
    println!("Welcome to Calculator CLI!");
    println!("Enter 'quit' to exit.");
    
    loop {
        print!("Enter expression (e.g., '5 + 3'): ");
        io::stdout().flush().unwrap();
        
        let mut input = String::new();
        io::stdin().read_line(&mut input).expect("Failed to read input");
        
        let input = input.trim();
        if input == "quit" {
            break;
        }
        
        let parts: Vec<&str> = input.split_whitespace().collect();
        if parts.len() != 3 {
            println!("Invalid format. Use: number operation number");
            continue;
        }
        
        let first: f64 = match parts[0].parse() {
            Ok(num) => num,
            Err(_) => {
                println!("Invalid first number");
                continue;
            }
        };
        
        let operation = parts[1];
        
        let second: f64 = match parts[2].parse() {
            Ok(num) => num,
            Err(_) => {
                println!("Invalid second number");
                continue;
            }
        };
        
        match calculate(operation, first, second) {
            Ok(result) => println!("Result: {}", result),
            Err(error) => println!("{}", error),
        }
    }
}

fn main() {
    let cli = Cli::parse();
    
    // If no arguments provided, run in interactive mode
    if cli.operation.is_empty() {
        interactive_mode();
        return;
    }
    
    match calculate(&cli.operation, cli.first, cli.second) {
        Ok(result) => println!("{} {} {} = {}", cli.first, cli.operation, cli.second, result),
        Err(error) => println!("{}", error),
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_addition() {
        assert_eq!(calculate("add", 5.0, 3.0).unwrap(), 8.0);
    }

    #[test]
    fn test_subtraction() {
        assert_eq!(calculate("subtract", 10.0, 4.0).unwrap(), 6.0);
    }

    #[test]
    fn test_multiplication() {
        assert_eq!(calculate("multiply", 6.0, 7.0).unwrap(), 42.0);
    }

    #[test]
    fn test_division() {
        assert_eq!(calculate("divide", 15.0, 3.0).unwrap(), 5.0);
    }

    #[test]
    fn test_division_by_zero() {
        assert!(calculate("divide", 5.0, 0.0).is_err());
    }

    #[test]
    fn test_invalid_operation() {
        assert!(calculate("invalid", 5.0, 3.0).is_err());
    }
}