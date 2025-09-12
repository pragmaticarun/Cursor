use clap::{Parser, Subcommand};
use criterion::{black_box, criterion_group, criterion_main, Criterion};
use rayon::prelude::*;
use serde::{Deserialize, Serialize};
use std::collections::HashMap;
use std::fs;
use std::io;
use std::time::Duration;
use std::time::Instant;
use thiserror::Error;

#[derive(Parser)]
#[command(name = "high-performance-parser")]
#[command(about = "A high-performance parser demonstrating optimization techniques")]
struct Cli {
    #[command(subcommand)]
    command: Commands,
}

#[derive(Subcommand)]
enum Commands {
    /// Parse a JSON file
    ParseJson {
        input: String,
        #[arg(short, long)]
        output: Option<String>,
        #[arg(short, long)]
        benchmark: bool,
    },
    /// Parse a CSV file
    ParseCsv {
        input: String,
        #[arg(short, long)]
        output: Option<String>,
        #[arg(short, long)]
        benchmark: bool,
    },
    /// Parse a log file
    ParseLog {
        input: String,
        #[arg(short, long)]
        pattern: Option<String>,
        #[arg(short, long)]
        output: Option<String>,
        #[arg(short, long)]
        benchmark: bool,
    },
    /// Run performance benchmarks
    Benchmark {
        #[arg(short, long)]
        iterations: Option<usize>,
    },
    /// Generate test data
    Generate {
        #[command(subcommand)]
        data_type: DataType,
        #[arg(short, long)]
        size: Option<usize>,
        #[arg(short, long)]
        output: String,
    },
}

#[derive(Debug, Subcommand)]
enum DataType {
    Json,
    Csv,
    Log,
}

#[derive(Error, Debug)]
enum ParserError {
    #[error("IO error: {0}")]
    IoError(#[from] io::Error),
    #[error("JSON parse error: {0}")]
    JsonError(#[from] serde_json::Error),
    #[error("CSV parse error: {0}")]
    CsvError(String),
    #[error("Parse error: {0}")]
    ParseError(String),
}

// High-performance JSON parser
struct JsonParser;

impl JsonParser {
    fn parse_file_fast(path: &str) -> Result<Vec<serde_json::Value>, ParserError> {
        let content = fs::read_to_string(path)?;
        Self::parse_content_fast(&content)
    }
    
    fn parse_content_fast(content: &str) -> Result<Vec<serde_json::Value>, ParserError> {
        // Optimized parsing for large JSON arrays
        let mut results = Vec::new();
        let mut chars = content.chars().peekable();
        
        // Skip whitespace
        while chars.peek().map_or(false, |c| c.is_whitespace()) {
            chars.next();
        }
        
        // Handle array or single object
        if chars.peek() == Some(&'[') {
            chars.next(); // Skip '['
            Self::parse_array_fast(&mut chars, &mut results)?;
        } else {
            // Single object
            let obj_str = Self::extract_object(&mut chars)?;
            let obj: serde_json::Value = serde_json::from_str(&obj_str)?;
            results.push(obj);
        }
        
        Ok(results)
    }
    
    fn parse_array_fast(chars: &mut std::iter::Peekable<std::str::Chars>, results: &mut Vec<serde_json::Value>) -> Result<(), ParserError> {
        while let Some(&ch) = chars.peek() {
            if ch == ']' {
                chars.next();
                break;
            }
            
            if ch.is_whitespace() || ch == ',' {
                chars.next();
                continue;
            }
            
            let obj_str = Self::extract_object(chars)?;
            let obj: serde_json::Value = serde_json::from_str(&obj_str)?;
            results.push(obj);
        }
        Ok(())
    }
    
    fn extract_object(chars: &mut std::iter::Peekable<std::str::Chars>) -> Result<String, ParserError> {
        let mut result = String::new();
        let mut brace_count = 0;
        let mut in_string = false;
        let mut escape_next = false;
        
        while let Some(ch) = chars.next() {
            if escape_next {
                escape_next = false;
                result.push(ch);
                continue;
            }
            
            match ch {
                '\\' if in_string => {
                    escape_next = true;
                    result.push(ch);
                }
                '"' => {
                    in_string = !in_string;
                    result.push(ch);
                }
                '{' if !in_string => {
                    brace_count += 1;
                    result.push(ch);
                }
                '}' if !in_string => {
                    brace_count -= 1;
                    result.push(ch);
                    if brace_count == 0 {
                        break;
                    }
                }
                _ => result.push(ch),
            }
        }
        
        Ok(result)
    }
    
    fn parse_parallel(content: &str) -> Result<Vec<serde_json::Value>, ParserError> {
        // Split content into chunks and parse in parallel
        let lines: Vec<&str> = content.lines().collect();
        let chunk_size = (lines.len() / rayon::current_num_threads()).max(1);
        
        let results: Result<Vec<_>, _> = lines
            .par_chunks(chunk_size)
            .map(|chunk| {
                let chunk_content = chunk.join("\n");
                Self::parse_content_fast(&chunk_content)
            })
            .collect();
        
        let mut final_results = Vec::new();
        for chunk_results in results? {
            final_results.extend(chunk_results);
        }
        
        Ok(final_results)
    }
}

// High-performance CSV parser
struct CsvParser;

impl CsvParser {
    fn parse_file_fast(path: &str) -> Result<Vec<HashMap<String, String>>, ParserError> {
        let content = fs::read_to_string(path)?;
        Self::parse_content_fast(&content)
    }
    
    fn parse_content_fast(content: &str) -> Result<Vec<HashMap<String, String>>, ParserError> {
        let mut lines = content.lines();
        let header_line = lines.next().ok_or_else(|| ParserError::ParseError("Empty CSV file".to_string()))?;
        let headers: Vec<String> = Self::parse_csv_line(header_line);
        
        let mut records = Vec::new();
        for line in lines {
            if line.trim().is_empty() {
                continue;
            }
            let values = Self::parse_csv_line(line);
            if values.len() != headers.len() {
                return Err(ParserError::ParseError("Column count mismatch".to_string()));
            }
            
            let mut record = HashMap::new();
            for (i, value) in values.iter().enumerate() {
                record.insert(headers[i].clone(), value.clone());
            }
            records.push(record);
        }
        
        Ok(records)
    }
    
    fn parse_csv_line(line: &str) -> Vec<String> {
        let mut fields = Vec::new();
        let mut current_field = String::new();
        let mut in_quotes = false;
        let mut chars = line.chars().peekable();
        
        while let Some(ch) = chars.next() {
            match ch {
                '"' => {
                    if in_quotes && chars.peek() == Some(&'"') {
                        // Escaped quote
                        chars.next();
                        current_field.push('"');
                    } else {
                        in_quotes = !in_quotes;
                    }
                }
                ',' if !in_quotes => {
                    fields.push(current_field.trim().to_string());
                    current_field.clear();
                }
                _ => current_field.push(ch),
            }
        }
        
        fields.push(current_field.trim().to_string());
        fields
    }
    
    fn parse_parallel(content: &str) -> Result<Vec<HashMap<String, String>>, ParserError> {
        let lines: Vec<&str> = content.lines().collect();
        if lines.is_empty() {
            return Err(ParserError::ParseError("Empty CSV file".to_string()));
        }
        
        let header_line = lines[0];
        let headers: Vec<String> = Self::parse_csv_line(header_line);
        let data_lines = &lines[1..];
        
        let chunk_size = (data_lines.len() / rayon::current_num_threads()).max(1);
        
        let results: Result<Vec<_>, _> = data_lines
            .par_chunks(chunk_size)
            .map(|chunk| {
                let mut records = Vec::new();
                for line in chunk {
                    if line.trim().is_empty() {
                        continue;
                    }
                    let values = Self::parse_csv_line(line);
            if values.len() == headers.len() {
                let mut record = HashMap::new();
                for (i, value) in values.iter().enumerate() {
                    record.insert(headers[i].clone(), value.clone());
                }
                records.push(record);
            }
                }
                Ok::<Vec<HashMap<String, String>>, ParserError>(records)
            })
            .collect();
        
        let mut final_results = Vec::new();
        for chunk_results in results? {
            final_results.extend(chunk_results);
        }
        
        Ok(final_results)
    }
}

// High-performance log parser
struct LogParser;

#[derive(Debug, Clone, Serialize, Deserialize)]
struct LogEntry {
    timestamp: String,
    level: String,
    message: String,
    source: Option<String>,
}

impl LogParser {
    fn parse_file_fast(path: &str, pattern: Option<&str>) -> Result<Vec<LogEntry>, ParserError> {
        let content = fs::read_to_string(path)?;
        Self::parse_content_fast(&content, pattern)
    }
    
    fn parse_content_fast(content: &str, pattern: Option<&str>) -> Result<Vec<LogEntry>, ParserError> {
        let mut entries = Vec::new();
        
        for line in content.lines() {
            if let Some(entry) = Self::parse_log_line(line, pattern) {
                entries.push(entry);
            }
        }
        
        Ok(entries)
    }
    
    fn parse_log_line(line: &str, pattern: Option<&str>) -> Option<LogEntry> {
        // Common log formats: [timestamp] level: message
        if let Some(pattern) = pattern {
            if !line.contains(pattern) {
                return None;
            }
        }
        
        // Try to parse timestamp [YYYY-MM-DD HH:MM:SS]
        let timestamp_start = line.find('[')?;
        let timestamp_end = line.find(']')?;
        let timestamp = line[timestamp_start + 1..timestamp_end].to_string();
        
        // Find level (INFO, ERROR, WARN, DEBUG)
        let after_timestamp = &line[timestamp_end + 1..];
        let level_start = after_timestamp.find(|c: char| c.is_alphabetic())?;
        let level_end = after_timestamp[level_start..].find(' ').unwrap_or(after_timestamp.len());
        let level = after_timestamp[level_start..level_start + level_end].to_string();
        
        // Rest is the message
        let message_start = timestamp_end + 1 + level_start + level_end;
        let message = if message_start < line.len() {
            line[message_start..].trim().to_string()
        } else {
            String::new()
        };
        
        Some(LogEntry {
            timestamp,
            level,
            message,
            source: None,
        })
    }
    
    fn parse_parallel(content: &str, pattern: Option<&str>) -> Result<Vec<LogEntry>, ParserError> {
        let lines: Vec<&str> = content.lines().collect();
        let chunk_size = (lines.len() / rayon::current_num_threads()).max(1);
        
        let results: Vec<_> = lines
            .par_chunks(chunk_size)
            .map(|chunk| {
                chunk.iter()
                    .filter_map(|line| Self::parse_log_line(line, pattern))
                    .collect::<Vec<_>>()
            })
            .collect();
        
        let mut final_results = Vec::new();
        for chunk_results in results {
            final_results.extend(chunk_results);
        }
        
        Ok(final_results)
    }
}

// Benchmark functions
fn benchmark_json_parsing(c: &mut Criterion) {
    let test_data = generate_test_json(10000);
    
    c.bench_function("json_parse_sequential", |b| {
        b.iter(|| {
            JsonParser::parse_content_fast(black_box(&test_data))
        })
    });
    
    c.bench_function("json_parse_parallel", |b| {
        b.iter(|| {
            JsonParser::parse_parallel(black_box(&test_data))
        })
    });
}

fn benchmark_csv_parsing(c: &mut Criterion) {
    let test_data = generate_test_csv(10000);
    
    c.bench_function("csv_parse_sequential", |b| {
        b.iter(|| {
            CsvParser::parse_content_fast(black_box(&test_data))
        })
    });
    
    c.bench_function("csv_parse_parallel", |b| {
        b.iter(|| {
            CsvParser::parse_parallel(black_box(&test_data))
        })
    });
}

fn benchmark_log_parsing(c: &mut Criterion) {
    let test_data = generate_test_logs(10000);
    
    c.bench_function("log_parse_sequential", |b| {
        b.iter(|| {
            LogParser::parse_content_fast(black_box(&test_data), black_box(None))
        })
    });
    
    c.bench_function("log_parse_parallel", |b| {
        b.iter(|| {
            LogParser::parse_parallel(black_box(&test_data), black_box(None))
        })
    });
}

criterion_group!(benches, benchmark_json_parsing, benchmark_csv_parsing, benchmark_log_parsing);
// criterion_main!(benches); // Commented out to avoid main function conflict

// Test data generators
fn generate_test_json(count: usize) -> String {
    let mut json = String::from("[");
    
    for i in 0..count {
        if i > 0 {
            json.push(',');
        }
        json.push_str(&format!(
            r#"{{"id":{},"name":"User{}","email":"user{}@example.com","active":true,"score":{:.2}}}"#,
            i, i, i, (i as f64) * 0.1
        ));
    }
    
    json.push(']');
    json
}

fn generate_test_csv(count: usize) -> String {
    let mut csv = String::from("id,name,email,active,score\n");
    
    for i in 0..count {
        csv.push_str(&format!(
            "{},User{},user{}@example.com,true,{:.2}\n",
            i, i, i, (i as f64) * 0.1
        ));
    }
    
    csv
}

fn generate_test_logs(count: usize) -> String {
    let mut logs = String::new();
    let levels = ["INFO", "ERROR", "WARN", "DEBUG"];
    
    for i in 0..count {
        let level = levels[i % levels.len()];
        logs.push_str(&format!(
            "[2024-01-{:02} 12:00:00] {}: This is a test log message number {}\n",
            (i % 28) + 1, level, i
        ));
    }
    
    logs
}

fn run_performance_test<T, F>(name: &str, iterations: usize, test_fn: F) -> Result<(), ParserError>
where
    F: Fn() -> Result<T, ParserError>,
{
    println!("Running {} performance test ({} iterations)...", name, iterations);
    
    let start = Instant::now();
    let mut total_time = Duration::new(0, 0);
    
    for i in 0..iterations {
        let iter_start = Instant::now();
        test_fn()?;
        let iter_duration = iter_start.elapsed();
        total_time += iter_duration;
        
        if i % 100 == 0 {
            println!("  Iteration {}: {:?}", i, iter_duration);
        }
    }
    
    let avg_time = total_time / iterations as u32;
    println!("{} - Average time: {:?}", name, avg_time);
    println!("{} - Total time: {:?}", name, total_time);
    
    Ok(())
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let cli = Cli::parse();
    
    match cli.command {
        Commands::ParseJson { input, output, benchmark } => {
            let start = Instant::now();
            let results = JsonParser::parse_file_fast(&input)?;
            let duration = start.elapsed();
            
            println!("Parsed {} JSON objects in {:?}", results.len(), duration);
            
            if benchmark {
                run_performance_test("JSON Sequential", 100, || {
                    JsonParser::parse_file_fast(&input)
                })?;
                
                run_performance_test("JSON Parallel", 100, || {
                    let content = fs::read_to_string(&input)?;
                    JsonParser::parse_parallel(&content)
                })?;
            }
            
            if let Some(output_path) = output {
                let json = serde_json::to_string_pretty(&results)?;
                fs::write(&output_path, json)?;
                println!("Results saved to: {}", output_path);
            }
        }
        Commands::ParseCsv { input, output, benchmark } => {
            let start = Instant::now();
            let results = CsvParser::parse_file_fast(&input)?;
            let duration = start.elapsed();
            
            println!("Parsed {} CSV records in {:?}", results.len(), duration);
            
            if benchmark {
                run_performance_test("CSV Sequential", 100, || {
                    CsvParser::parse_file_fast(&input)
                })?;
                
                run_performance_test("CSV Parallel", 100, || {
                    let content = fs::read_to_string(&input)?;
                    CsvParser::parse_parallel(&content)
                })?;
            }
            
            if let Some(output_path) = output {
                let json = serde_json::to_string_pretty(&results)?;
                fs::write(&output_path, json)?;
                println!("Results saved to: {}", output_path);
            }
        }
        Commands::ParseLog { input, pattern, output, benchmark } => {
            let start = Instant::now();
            let results = LogParser::parse_file_fast(&input, pattern.as_deref())?;
            let duration = start.elapsed();
            
            println!("Parsed {} log entries in {:?}", results.len(), duration);
            
            if benchmark {
                run_performance_test("Log Sequential", 100, || {
                    LogParser::parse_file_fast(&input, pattern.as_deref())
                })?;
                
                run_performance_test("Log Parallel", 100, || {
                    let content = fs::read_to_string(&input)?;
                    LogParser::parse_parallel(&content, pattern.as_deref())
                })?;
            }
            
            if let Some(output_path) = output {
                let json = serde_json::to_string_pretty(&results)?;
                fs::write(&output_path, json)?;
                println!("Results saved to: {}", output_path);
            }
        }
        Commands::Benchmark { iterations } => {
            let iterations = iterations.unwrap_or(1000);
            
            println!("Running comprehensive benchmarks...");
            
            // Generate test data
            let json_data = generate_test_json(1000);
            let csv_data = generate_test_csv(1000);
            let log_data = generate_test_logs(1000);
            
            // JSON benchmarks
            run_performance_test("JSON Sequential", iterations, || {
                JsonParser::parse_content_fast(&json_data)
            })?;
            
            run_performance_test("JSON Parallel", iterations, || {
                JsonParser::parse_parallel(&json_data)
            })?;
            
            // CSV benchmarks
            run_performance_test("CSV Sequential", iterations, || {
                CsvParser::parse_content_fast(&csv_data)
            })?;
            
            run_performance_test("CSV Parallel", iterations, || {
                CsvParser::parse_parallel(&csv_data)
            })?;
            
            // Log benchmarks
            run_performance_test("Log Sequential", iterations, || {
                LogParser::parse_content_fast(&log_data, None)
            })?;
            
            run_performance_test("Log Parallel", iterations, || {
                LogParser::parse_parallel(&log_data, None)
            })?;
        }
        Commands::Generate { data_type, size, output } => {
            let size = size.unwrap_or(1000);
            
            let data = match data_type {
                DataType::Json => generate_test_json(size),
                DataType::Csv => generate_test_csv(size),
                DataType::Log => generate_test_logs(size),
            };
            
            fs::write(&output, data)?;
            println!("Generated {} records of {:?} data to: {}", size, data_type, output);
        }
    }
    
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_json_parser() {
        let json_data = r#"[{"id":1,"name":"test"},{"id":2,"name":"test2"}]"#;
        let results = JsonParser::parse_content_fast(json_data).unwrap();
        assert_eq!(results.len(), 2);
        assert_eq!(results[0]["id"], 1);
        assert_eq!(results[0]["name"], "test");
    }

    #[test]
    fn test_csv_parser() {
        let csv_data = "id,name,email\n1,John,john@example.com\n2,Jane,jane@example.com";
        let results = CsvParser::parse_content_fast(csv_data).unwrap();
        assert_eq!(results.len(), 2);
        assert_eq!(results[0]["name"], "John");
        assert_eq!(results[1]["email"], "jane@example.com");
    }

    #[test]
    fn test_log_parser() {
        let log_data = "[2024-01-01 12:00:00] INFO: This is a test message";
        let results = LogParser::parse_content_fast(log_data, None).unwrap();
        assert_eq!(results.len(), 1);
        assert_eq!(results[0].level, "INFO");
        assert_eq!(results[0].message, "This is a test message");
    }

    #[test]
    fn test_parallel_parsing() {
        let json_data = generate_test_json(100);
        let sequential = JsonParser::parse_content_fast(&json_data).unwrap();
        let parallel = JsonParser::parse_parallel(&json_data).unwrap();
        assert_eq!(sequential.len(), parallel.len());
    }
}