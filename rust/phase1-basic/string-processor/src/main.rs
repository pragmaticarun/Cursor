use clap::Parser;
use regex::Regex;
use std::fs;
use std::io::{self, Write};

#[derive(Parser)]
#[command(name = "string-processor")]
#[command(about = "A string processing tool demonstrating ownership and borrowing")]
struct Cli {
    /// Input file path
    #[arg(short, long)]
    input: String,
    
    /// Output file path
    #[arg(short, long)]
    output: Option<String>,
    
    /// Operation to perform
    #[arg(long)]
    operation: String,
    
    /// Pattern for search/replace operations
    #[arg(short, long)]
    pattern: Option<String>,
    
    /// Replacement text for replace operations
    #[arg(short, long)]
    replacement: Option<String>,
}

#[derive(Debug)]
enum ProcessorError {
    FileNotFound,
    InvalidOperation,
    RegexError(String),
    IoError(String),
}

impl std::fmt::Display for ProcessorError {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        match self {
            ProcessorError::FileNotFound => write!(f, "File not found"),
            ProcessorError::InvalidOperation => write!(f, "Invalid operation"),
            ProcessorError::RegexError(msg) => write!(f, "Regex error: {}", msg),
            ProcessorError::IoError(msg) => write!(f, "IO error: {}", msg),
        }
    }
}

struct StringProcessor {
    content: String,
}

impl StringProcessor {
    fn new(content: String) -> Self {
        Self { content }
    }
    
    fn from_file(path: &str) -> Result<Self, ProcessorError> {
        let content = fs::read_to_string(path)
            .map_err(|e| ProcessorError::IoError(e.to_string()))?;
        Ok(Self::new(content))
    }
    
    fn to_file(&self, path: &str) -> Result<(), ProcessorError> {
        fs::write(path, &self.content)
            .map_err(|e| ProcessorError::IoError(e.to_string()))?;
        Ok(())
    }
    
    fn count_words(&self) -> usize {
        self.content.split_whitespace().count()
    }
    
    fn count_lines(&self) -> usize {
        self.content.lines().count()
    }
    
    fn count_chars(&self) -> usize {
        self.content.chars().count()
    }
    
    fn to_uppercase(&mut self) {
        self.content = self.content.to_uppercase();
    }
    
    fn to_lowercase(&mut self) {
        self.content = self.content.to_lowercase();
    }
    
    fn reverse(&mut self) {
        self.content = self.content.chars().rev().collect();
    }
    
    fn replace(&mut self, pattern: &str, replacement: &str) -> Result<usize, ProcessorError> {
        let regex = Regex::new(pattern)
            .map_err(|e| ProcessorError::RegexError(e.to_string()))?;
        
        let count = regex.find_iter(&self.content).count();
        self.content = regex.replace_all(&self.content, replacement).to_string();
        Ok(count)
    }
    
    fn remove_whitespace(&mut self) {
        self.content = self.content.split_whitespace().collect::<Vec<&str>>().join(" ");
    }
    
    fn get_stats(&self) -> String {
        format!(
            "Statistics:\n- Characters: {}\n- Words: {}\n- Lines: {}",
            self.count_chars(),
            self.count_words(),
            self.count_lines()
        )
    }
}

fn process_file(cli: &Cli) -> Result<(), ProcessorError> {
    let mut processor = StringProcessor::from_file(&cli.input)?;
    
    match cli.operation.as_str() {
        "count" => {
            println!("{}", processor.get_stats());
        }
        "uppercase" => {
            processor.to_uppercase();
            if let Some(output) = &cli.output {
                processor.to_file(output)?;
                println!("Converted to uppercase and saved to {}", output);
            } else {
                println!("{}", processor.content);
            }
        }
        "lowercase" => {
            processor.to_lowercase();
            if let Some(output) = &cli.output {
                processor.to_file(output)?;
                println!("Converted to lowercase and saved to {}", output);
            } else {
                println!("{}", processor.content);
            }
        }
        "reverse" => {
            processor.reverse();
            if let Some(output) = &cli.output {
                processor.to_file(output)?;
                println!("Reversed and saved to {}", output);
            } else {
                println!("{}", processor.content);
            }
        }
        "replace" => {
            if let (Some(pattern), Some(replacement)) = (&cli.pattern, &cli.replacement) {
                let count = processor.replace(pattern, replacement)?;
                if let Some(output) = &cli.output {
                    processor.to_file(output)?;
                    println!("Replaced {} occurrences and saved to {}", count, output);
                } else {
                    println!("{}", processor.content);
                }
            } else {
                return Err(ProcessorError::InvalidOperation);
            }
        }
        "clean" => {
            processor.remove_whitespace();
            if let Some(output) = &cli.output {
                processor.to_file(output)?;
                println!("Cleaned whitespace and saved to {}", output);
            } else {
                println!("{}", processor.content);
            }
        }
        _ => return Err(ProcessorError::InvalidOperation),
    }
    
    Ok(())
}

fn interactive_mode() {
    println!("Welcome to String Processor!");
    println!("Available operations: count, uppercase, lowercase, reverse, replace, clean");
    println!("Enter 'quit' to exit.");
    
    loop {
        print!("Enter text to process: ");
        io::stdout().flush().unwrap();
        
        let mut input = String::new();
        io::stdin().read_line(&mut input).expect("Failed to read input");
        
        let input = input.trim();
        if input == "quit" {
            break;
        }
        
        let mut processor = StringProcessor::new(input.to_string());
        
        print!("Enter operation: ");
        io::stdout().flush().unwrap();
        
        let mut operation = String::new();
        io::stdin().read_line(&mut operation).expect("Failed to read input");
        
        match operation.trim() {
            "count" => println!("{}", processor.get_stats()),
            "uppercase" => {
                processor.to_uppercase();
                println!("{}", processor.content);
            }
            "lowercase" => {
                processor.to_lowercase();
                println!("{}", processor.content);
            }
            "reverse" => {
                processor.reverse();
                println!("{}", processor.content);
            }
            "clean" => {
                processor.remove_whitespace();
                println!("{}", processor.content);
            }
            _ => println!("Invalid operation"),
        }
    }
}

fn main() {
    let cli = Cli::parse();
    
    // If no input file provided, run in interactive mode
    if cli.input.is_empty() {
        interactive_mode();
        return;
    }
    
    match process_file(&cli) {
        Ok(_) => {}
        Err(error) => {
            eprintln!("Error: {}", error);
            std::process::exit(1);
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_word_count() {
        let processor = StringProcessor::new("Hello world from Rust".to_string());
        assert_eq!(processor.count_words(), 4);
    }

    #[test]
    fn test_line_count() {
        let processor = StringProcessor::new("Line 1\nLine 2\nLine 3".to_string());
        assert_eq!(processor.count_lines(), 3);
    }

    #[test]
    fn test_uppercase() {
        let mut processor = StringProcessor::new("hello world".to_string());
        processor.to_uppercase();
        assert_eq!(processor.content, "HELLO WORLD");
    }

    #[test]
    fn test_replace() {
        let mut processor = StringProcessor::new("Hello world".to_string());
        let count = processor.replace("world", "Rust").unwrap();
        assert_eq!(count, 1);
        assert_eq!(processor.content, "Hello Rust");
    }
}