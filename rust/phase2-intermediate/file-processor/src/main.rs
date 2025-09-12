use clap::{Parser, Subcommand};
use csv::{Reader, Writer};
use regex::Regex;
use serde::{Deserialize, Serialize};
use std::fs;
use std::io;
use std::path::Path;
use walkdir::WalkDir;
use thiserror::Error;

#[derive(Parser)]
#[command(name = "file-processor")]
#[command(about = "A robust file processor demonstrating error handling patterns")]
struct Cli {
    #[command(subcommand)]
    command: Commands,
}

#[derive(Subcommand)]
enum Commands {
    /// Process a single file
    Process {
        input: String,
        output: Option<String>,
        #[command(subcommand)]
        operation: ProcessOperation,
    },
    /// Process multiple files in a directory
    Batch {
        directory: String,
        pattern: Option<String>,
        #[command(subcommand)]
        operation: ProcessOperation,
    },
    /// Convert between file formats
    Convert {
        input: String,
        output: String,
        #[arg(short, long)]
        from: String,
        #[arg(short, long)]
        to: String,
    },
    /// Analyze file statistics
    Analyze {
        input: String,
    },
}

#[derive(Subcommand)]
enum ProcessOperation {
    /// Count words, lines, characters
    Count,
    /// Convert to uppercase
    Uppercase,
    /// Convert to lowercase
    Lowercase,
    /// Remove extra whitespace
    Clean,
    /// Replace text using regex
    Replace {
        pattern: String,
        replacement: String,
    },
    /// Extract lines matching pattern
    Extract {
        pattern: String,
    },
    /// Sort lines
    Sort,
    /// Remove duplicate lines
    Dedup,
}

#[derive(Error, Debug)]
enum ProcessorError {
    #[error("File not found: {0}")]
    FileNotFound(String),
    #[error("IO error: {0}")]
    IoError(#[from] io::Error),
    #[error("CSV error: {0}")]
    CsvError(#[from] csv::Error),
    #[error("Regex error: {0}")]
    RegexError(#[from] regex::Error),
    #[error("JSON error: {0}")]
    JsonError(#[from] serde_json::Error),
    #[error("Invalid file format: {0}")]
    InvalidFormat(String),
    #[error("Processing error: {0}")]
    ProcessingError(String),
    #[error("Walkdir error: {0}")]
    WalkdirError(#[from] walkdir::Error),
}

#[derive(Debug, Serialize, Deserialize)]
struct FileStats {
    filename: String,
    size_bytes: u64,
    lines: usize,
    words: usize,
    characters: usize,
    empty_lines: usize,
    longest_line: usize,
    file_type: String,
}

impl FileStats {
    fn new(filename: String, content: &str) -> Self {
        let lines: Vec<&str> = content.lines().collect();
        let words: usize = content.split_whitespace().count();
        let characters = content.chars().count();
        let empty_lines = lines.iter().filter(|line| line.trim().is_empty()).count();
        let longest_line = lines.iter().map(|line| line.len()).max().unwrap_or(0);
        
        let file_type = Path::new(&filename)
            .extension()
            .and_then(|ext| ext.to_str())
            .unwrap_or("unknown")
            .to_string();
        
        Self {
            filename,
            size_bytes: content.len() as u64,
            lines: lines.len(),
            words,
            characters,
            empty_lines,
            longest_line,
            file_type,
        }
    }
}

struct FileProcessor;

impl FileProcessor {
    fn read_file(path: &str) -> Result<String, ProcessorError> {
        fs::read_to_string(path)
            .map_err(|_| ProcessorError::FileNotFound(path.to_string()))
    }
    
    fn write_file(path: &str, content: &str) -> Result<(), ProcessorError> {
        fs::write(path, content)?;
        Ok(())
    }
    
    fn process_content(content: &str, operation: &ProcessOperation) -> Result<String, ProcessorError> {
        match operation {
            ProcessOperation::Count => {
                let stats = FileStats::new("".to_string(), content);
                Ok(format!(
                    "Lines: {}\nWords: {}\nCharacters: {}\nEmpty lines: {}\nLongest line: {}",
                    stats.lines, stats.words, stats.characters, stats.empty_lines, stats.longest_line
                ))
            }
            ProcessOperation::Uppercase => Ok(content.to_uppercase()),
            ProcessOperation::Lowercase => Ok(content.to_lowercase()),
            ProcessOperation::Clean => {
                let cleaned: Vec<&str> = content.lines()
                    .map(|line| line.trim())
                    .filter(|line| !line.is_empty())
                    .collect();
                Ok(cleaned.join("\n"))
            }
            ProcessOperation::Replace { pattern, replacement } => {
                let regex = Regex::new(pattern)?;
                Ok(regex.replace_all(content, replacement).to_string())
            }
            ProcessOperation::Extract { pattern } => {
                let regex = Regex::new(pattern)?;
                let matching_lines: Vec<&str> = content.lines()
                    .filter(|line| regex.is_match(line))
                    .collect();
                Ok(matching_lines.join("\n"))
            }
            ProcessOperation::Sort => {
                let mut lines: Vec<&str> = content.lines().collect();
                lines.sort();
                Ok(lines.join("\n"))
            }
            ProcessOperation::Dedup => {
                let mut seen = std::collections::HashSet::new();
                let unique_lines: Vec<String> = content.lines()
                    .filter(|line| seen.insert(*line))
                    .map(|line| line.to_string())
                    .collect();
                Ok(unique_lines.join("\n"))
            }
        }
    }
    
    fn convert_format(input: &str, output: &str, from: &str, to: &str) -> Result<(), ProcessorError> {
        match (from, to) {
            ("txt", "json") => {
                let content = Self::read_file(input)?;
                let stats = FileStats::new(input.to_string(), &content);
                let json = serde_json::to_string_pretty(&stats)?;
                Self::write_file(output, &json)?;
            }
            ("csv", "json") => {
                let mut reader = Reader::from_path(input)?;
                let mut records: Vec<std::collections::HashMap<String, String>> = Vec::new();
                
                for result in reader.records() {
                    let record = result?;
                    let mut map = std::collections::HashMap::new();
                    for (i, field) in record.iter().enumerate() {
                        map.insert(format!("column_{}", i), field.to_string());
                    }
                    records.push(map);
                }
                
                let json = serde_json::to_string_pretty(&records)?;
                Self::write_file(output, &json)?;
            }
            ("json", "csv") => {
                let content = Self::read_file(input)?;
                let data: Vec<std::collections::HashMap<String, String>> = serde_json::from_str(&content)?;
                
                if data.is_empty() {
                    return Err(ProcessorError::ProcessingError("No data to convert".to_string()));
                }
                
                let mut writer = Writer::from_path(output)?;
                
                // Write headers
                let headers: Vec<String> = data[0].keys().cloned().collect();
                writer.write_record(&headers)?;
                
                // Write data
                for record in &data {
                    let values: Vec<String> = headers.iter()
                        .map(|header| record.get(header).unwrap_or(&String::new()).clone())
                        .collect();
                    writer.write_record(&values)?;
                }
                
                writer.flush()?;
            }
            _ => return Err(ProcessorError::InvalidFormat(format!("Conversion from {} to {} not supported", from, to))),
        }
        
        Ok(())
    }
    
    fn analyze_file(path: &str) -> Result<FileStats, ProcessorError> {
        let content = Self::read_file(path)?;
        Ok(FileStats::new(path.to_string(), &content))
    }
    
    fn batch_process(directory: &str, pattern: Option<&str>, operation: &ProcessOperation) -> Result<(), ProcessorError> {
        let walker = WalkDir::new(directory).into_iter();
        
        for entry in walker {
            let entry = entry?;
            let path = entry.path();
            
            if path.is_file() {
                let path_str = path.to_string_lossy();
                
                // Apply pattern filter if provided
                if let Some(pattern) = pattern {
                    let regex = Regex::new(pattern)?;
                    if !regex.is_match(&path_str) {
                        continue;
                    }
                }
                
                println!("Processing: {}", path_str);
                
                match Self::read_file(&path_str) {
                    Ok(content) => {
                        match Self::process_content(&content, operation) {
                            Ok(result) => {
                                let output_path = format!("{}.processed", path_str);
                                Self::write_file(&output_path, &result)?;
                                println!("  -> Saved to: {}", output_path);
                            }
                            Err(e) => {
                                eprintln!("  -> Error processing {}: {}", path_str, e);
                            }
                        }
                    }
                    Err(e) => {
                        eprintln!("  -> Error reading {}: {}", path_str, e);
                    }
                }
            }
        }
        
        Ok(())
    }
}

fn main() {
    let cli = Cli::parse();
    
    match cli.command {
        Commands::Process { input, output, operation } => {
            match FileProcessor::read_file(&input) {
                Ok(content) => {
                    match FileProcessor::process_content(&content, &operation) {
                        Ok(result) => {
                            if let Some(output_path) = output {
                                match FileProcessor::write_file(&output_path, &result) {
                                    Ok(_) => println!("Processed file saved to: {}", output_path),
                                    Err(e) => eprintln!("Error writing file: {}", e),
                                }
                            } else {
                                println!("{}", result);
                            }
                        }
                        Err(e) => eprintln!("Error processing content: {}", e),
                    }
                }
                Err(e) => eprintln!("Error reading file: {}", e),
            }
        }
        Commands::Batch { directory, pattern, operation } => {
            match FileProcessor::batch_process(&directory, pattern.as_deref(), &operation) {
                Ok(_) => println!("Batch processing completed"),
                Err(e) => eprintln!("Error in batch processing: {}", e),
            }
        }
        Commands::Convert { input, output, from, to } => {
            match FileProcessor::convert_format(&input, &output, &from, &to) {
                Ok(_) => println!("Conversion completed: {} -> {}", input, output),
                Err(e) => eprintln!("Error converting file: {}", e),
            }
        }
        Commands::Analyze { input } => {
            match FileProcessor::analyze_file(&input) {
                Ok(stats) => {
                    println!("File Analysis for: {}", stats.filename);
                    println!("  Size: {} bytes", stats.size_bytes);
                    println!("  Lines: {}", stats.lines);
                    println!("  Words: {}", stats.words);
                    println!("  Characters: {}", stats.characters);
                    println!("  Empty lines: {}", stats.empty_lines);
                    println!("  Longest line: {} characters", stats.longest_line);
                    println!("  File type: {}", stats.file_type);
                }
                Err(e) => eprintln!("Error analyzing file: {}", e),
            }
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_file_stats() {
        let content = "Hello world\nThis is a test\n\nAnother line";
        let stats = FileStats::new("test.txt".to_string(), content);
        
        assert_eq!(stats.lines, 4);
        assert_eq!(stats.words, 7);
        assert_eq!(stats.empty_lines, 1);
    }

    #[test]
    fn test_process_content_uppercase() {
        let content = "hello world";
        let result = FileProcessor::process_content(content, &ProcessOperation::Uppercase).unwrap();
        assert_eq!(result, "HELLO WORLD");
    }

    #[test]
    fn test_process_content_clean() {
        let content = "  hello  \n\n  world  \n";
        let result = FileProcessor::process_content(content, &ProcessOperation::Clean).unwrap();
        assert_eq!(result, "hello\nworld");
    }

    #[test]
    fn test_process_content_replace() {
        let content = "hello world";
        let result = FileProcessor::process_content(
            content, 
            &ProcessOperation::Replace { 
                pattern: "world".to_string(), 
                replacement: "rust".to_string() 
            }
        ).unwrap();
        assert_eq!(result, "hello rust");
    }
}