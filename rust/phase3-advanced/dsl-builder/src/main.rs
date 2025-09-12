use clap::{Parser, Subcommand};
use serde::{Deserialize, Serialize};
use std::collections::HashMap;
use std::fs;
use thiserror::Error;

#[derive(Parser)]
#[command(name = "dsl-builder")]
#[command(about = "A DSL builder demonstrating macros and metaprogramming")]
struct Cli {
    #[command(subcommand)]
    command: Commands,
}

#[derive(Subcommand)]
enum Commands {
    /// Create a configuration using the DSL
    Create {
        #[command(subcommand)]
        config_type: ConfigType,
    },
    /// Parse and validate a DSL file
    Parse {
        input: String,
        #[arg(short, long)]
        validate: bool,
    },
    /// Generate code from DSL
    Generate {
        input: String,
        #[arg(short, long)]
        output: Option<String>,
    },
    /// Run DSL examples
    Examples,
}

#[derive(Subcommand)]
enum ConfigType {
    /// Create a web server configuration
    WebServer {
        #[arg(short, long)]
        port: Option<u16>,
        #[arg(long)]
        host: Option<String>,
        #[arg(short, long)]
        ssl: bool,
    },
    /// Create a database configuration
    Database {
        #[arg(short, long)]
        host: String,
        #[arg(short, long)]
        port: u16,
        #[arg(short, long)]
        name: String,
        #[arg(short, long)]
        user: String,
    },
    /// Create an application configuration
    App {
        name: String,
        #[arg(short, long)]
        version: Option<String>,
        #[arg(short, long)]
        debug: bool,
    },
}

// DSL Macros
macro_rules! config {
    ($name:ident {
        $($field:ident: $value:expr),* $(,)?
    }) => {
        Config {
            name: stringify!($name).to_string(),
            fields: {
                let mut map = HashMap::new();
                $(
                    map.insert(stringify!($field).to_string(), ConfigValue::from($value));
                )*
                map
            },
        }
    };
}

macro_rules! server {
    (port: $port:expr) => {
        config!(server {
            port: $port,
            host: "localhost",
            ssl: false,
        })
    };
    (port: $port:expr, host: $host:expr) => {
        config!(server {
            port: $port,
            host: $host,
            ssl: false,
        })
    };
    (port: $port:expr, host: $host:expr, ssl: $ssl:expr) => {
        config!(server {
            port: $port,
            host: $host,
            ssl: $ssl,
        })
    };
}

macro_rules! database {
    ($host:expr, $port:expr, $name:expr, $user:expr) => {
        config!(database {
            host: $host,
            port: $port,
            name: $name,
            user: $user,
            password: "***",
        })
    };
}

macro_rules! app {
    ($name:expr) => {
        config!(app {
            name: $name,
            version: "1.0.0",
            debug: false,
        })
    };
    ($name:expr, version: $version:expr) => {
        config!(app {
            name: $name,
            version: $version,
            debug: false,
        })
    };
    ($name:expr, version: $version:expr, debug: $debug:expr) => {
        config!(app {
            name: $name,
            version: $version,
            debug: $debug,
        })
    };
}

// DSL Data Structures
#[derive(Debug, Clone, Serialize, Deserialize)]
enum ConfigValue {
    String(String),
    Number(i64),
    Float(f64),
    Boolean(bool),
    Array(Vec<ConfigValue>),
    Object(HashMap<String, ConfigValue>),
}

impl From<String> for ConfigValue {
    fn from(value: String) -> Self {
        ConfigValue::String(value)
    }
}

impl From<&str> for ConfigValue {
    fn from(value: &str) -> Self {
        ConfigValue::String(value.to_string())
    }
}

impl From<i64> for ConfigValue {
    fn from(value: i64) -> Self {
        ConfigValue::Number(value)
    }
}

impl From<i32> for ConfigValue {
    fn from(value: i32) -> Self {
        ConfigValue::Number(value as i64)
    }
}

impl From<u16> for ConfigValue {
    fn from(value: u16) -> Self {
        ConfigValue::Number(value as i64)
    }
}

impl From<f64> for ConfigValue {
    fn from(value: f64) -> Self {
        ConfigValue::Float(value)
    }
}

impl From<bool> for ConfigValue {
    fn from(value: bool) -> Self {
        ConfigValue::Boolean(value)
    }
}

#[derive(Debug, Clone, Serialize, Deserialize)]
struct Config {
    name: String,
    fields: HashMap<String, ConfigValue>,
}

impl Config {
    fn new(name: String) -> Self {
        Self {
            name,
            fields: HashMap::new(),
        }
    }
    
    fn set(&mut self, key: &str, value: ConfigValue) {
        self.fields.insert(key.to_string(), value);
    }
    
    fn get(&self, key: &str) -> Option<&ConfigValue> {
        self.fields.get(key)
    }
    
    fn get_string(&self, key: &str) -> Option<String> {
        match self.get(key) {
            Some(ConfigValue::String(s)) => Some(s.clone()),
            _ => None,
        }
    }
    
    fn get_number(&self, key: &str) -> Option<i64> {
        match self.get(key) {
            Some(ConfigValue::Number(n)) => Some(*n),
            _ => None,
        }
    }
    
    fn get_boolean(&self, key: &str) -> Option<bool> {
        match self.get(key) {
            Some(ConfigValue::Boolean(b)) => Some(*b),
            _ => None,
        }
    }
    
    fn validate(&self) -> Result<(), ValidationError> {
        match self.name.as_str() {
            "server" => self.validate_server(),
            "database" => self.validate_database(),
            "app" => self.validate_app(),
            _ => Err(ValidationError::UnknownConfigType(self.name.clone())),
        }
    }
    
    fn validate_server(&self) -> Result<(), ValidationError> {
        if self.get_number("port").is_none() {
            return Err(ValidationError::MissingField("port".to_string()));
        }
        if self.get_string("host").is_none() {
            return Err(ValidationError::MissingField("host".to_string()));
        }
        if self.get_boolean("ssl").is_none() {
            return Err(ValidationError::MissingField("ssl".to_string()));
        }
        Ok(())
    }
    
    fn validate_database(&self) -> Result<(), ValidationError> {
        let required_fields = ["host", "port", "name", "user"];
        for field in &required_fields {
            if self.get(field).is_none() {
                return Err(ValidationError::MissingField(field.to_string()));
            }
        }
        Ok(())
    }
    
    fn validate_app(&self) -> Result<(), ValidationError> {
        if self.get_string("name").is_none() {
            return Err(ValidationError::MissingField("name".to_string()));
        }
        Ok(())
    }
}

#[derive(Error, Debug)]
enum ValidationError {
    #[error("Missing required field: {0}")]
    MissingField(String),
    #[error("Unknown config type: {0}")]
    UnknownConfigType(String),
    #[error("Invalid value for field {0}: {1}")]
    InvalidValue(String, String),
}

// DSL Parser
struct DSLParser;

impl DSLParser {
    fn parse_file(path: &str) -> Result<Config, Box<dyn std::error::Error>> {
        let content = fs::read_to_string(path)?;
        Self::parse_content(&content)
    }
    
    fn parse_content(content: &str) -> Result<Config, Box<dyn std::error::Error>> {
        // Simple parser for our DSL format
        let lines: Vec<&str> = content.lines().collect();
        let mut config = Config::new("parsed".to_string());
        
        for line in lines {
            let line = line.trim();
            if line.is_empty() || line.starts_with('#') {
                continue;
            }
            
            if let Some((key, value)) = Self::parse_key_value(line) {
                config.set(&key, value);
            }
        }
        
        Ok(config)
    }
    
    fn parse_key_value(line: &str) -> Option<(String, ConfigValue)> {
        if let Some(pos) = line.find('=') {
            let key = line[..pos].trim().to_string();
            let value_str = line[pos + 1..].trim();
            
            let value = if value_str.starts_with('"') && value_str.ends_with('"') {
                ConfigValue::String(value_str[1..value_str.len() - 1].to_string())
            } else if value_str == "true" {
                ConfigValue::Boolean(true)
            } else if value_str == "false" {
                ConfigValue::Boolean(false)
            } else if let Ok(num) = value_str.parse::<i64>() {
                ConfigValue::Number(num)
            } else if let Ok(num) = value_str.parse::<f64>() {
                ConfigValue::Float(num)
            } else {
                ConfigValue::String(value_str.to_string())
            };
            
            Some((key, value))
        } else {
            None
        }
    }
}

// Code Generator
struct CodeGenerator;

impl CodeGenerator {
    fn generate_rust_struct(config: &Config) -> String {
        let mut code = String::new();
        
        code.push_str(&format!("// Generated struct for {}\n", config.name));
        code.push_str("#[derive(Debug, Clone, Serialize, Deserialize)]\n");
        code.push_str(&format!("pub struct {}Config {{\n", Self::to_pascal_case(&config.name)));
        
        for (key, value) in &config.fields {
            let field_type = Self::get_rust_type(value);
            let field_name = Self::to_snake_case(key);
            code.push_str(&format!("    pub {}: {},\n", field_name, field_type));
        }
        
        code.push_str("}\n\n");
        
        // Generate implementation
        code.push_str(&format!("impl {}Config {{\n", Self::to_pascal_case(&config.name)));
        code.push_str("    pub fn new() -> Self {\n");
        code.push_str("        Self {\n");
        
        for (key, value) in &config.fields {
            let field_name = Self::to_snake_case(key);
            let default_value = Self::get_default_value(value);
            code.push_str(&format!("            {}: {},\n", field_name, default_value));
        }
        
        code.push_str("        }\n");
        code.push_str("    }\n");
        code.push_str("}\n");
        
        code
    }
    
    fn generate_json_schema(config: &Config) -> String {
        let mut schema = serde_json::Map::new();
        schema.insert("type".to_string(), serde_json::Value::String("object".to_string()));
        
        let mut properties = serde_json::Map::new();
        let mut required = Vec::new();
        
        for (key, value) in &config.fields {
            properties.insert(key.clone(), Self::get_json_schema_type(value));
            required.push(key.clone());
        }
        
        schema.insert("properties".to_string(), serde_json::Value::Object(properties));
        schema.insert("required".to_string(), serde_json::Value::Array(
            required.into_iter().map(|s| serde_json::Value::String(s)).collect()
        ));
        
        serde_json::to_string_pretty(&schema).unwrap()
    }
    
    fn to_pascal_case(s: &str) -> String {
        let mut result = String::new();
        let mut capitalize = true;
        
        for c in s.chars() {
            if c == '_' {
                capitalize = true;
            } else if capitalize {
                result.push(c.to_uppercase().next().unwrap());
                capitalize = false;
            } else {
                result.push(c);
            }
        }
        
        result
    }
    
    fn to_snake_case(s: &str) -> String {
        s.to_string()
    }
    
    fn get_rust_type(value: &ConfigValue) -> String {
        match value {
            ConfigValue::String(_) => "String".to_string(),
            ConfigValue::Number(_) => "i64".to_string(),
            ConfigValue::Float(_) => "f64".to_string(),
            ConfigValue::Boolean(_) => "bool".to_string(),
            ConfigValue::Array(_) => "Vec<ConfigValue>".to_string(),
            ConfigValue::Object(_) => "HashMap<String, ConfigValue>".to_string(),
        }
    }
    
    fn get_default_value(value: &ConfigValue) -> String {
        match value {
            ConfigValue::String(s) => format!("\"{}\".to_string()", s),
            ConfigValue::Number(n) => n.to_string(),
            ConfigValue::Float(f) => f.to_string(),
            ConfigValue::Boolean(b) => b.to_string(),
            ConfigValue::Array(_) => "Vec::new()".to_string(),
            ConfigValue::Object(_) => "HashMap::new()".to_string(),
        }
    }
    
    fn get_json_schema_type(value: &ConfigValue) -> serde_json::Value {
        match value {
            ConfigValue::String(_) => serde_json::json!({"type": "string"}),
            ConfigValue::Number(_) => serde_json::json!({"type": "integer"}),
            ConfigValue::Float(_) => serde_json::json!({"type": "number"}),
            ConfigValue::Boolean(_) => serde_json::json!({"type": "boolean"}),
            ConfigValue::Array(_) => serde_json::json!({"type": "array"}),
            ConfigValue::Object(_) => serde_json::json!({"type": "object"}),
        }
    }
}

fn main() {
    let cli = Cli::parse();
    
    match cli.command {
        Commands::Create { config_type } => {
            match config_type {
                ConfigType::WebServer { port, host, ssl } => {
                    let config = server!(
                        port: port.unwrap_or(8080),
                        host: host.as_deref().unwrap_or("localhost"),
                        ssl: ssl
                    );
                    
                    println!("Created web server configuration:");
                    println!("{}", serde_json::to_string_pretty(&config).unwrap());
                    
                    if let Err(e) = config.validate() {
                        eprintln!("Validation error: {}", e);
                    }
                }
                ConfigType::Database { host, port, name, user } => {
                    let config = database!(host, port, name, user);
                    
                    println!("Created database configuration:");
                    println!("{}", serde_json::to_string_pretty(&config).unwrap());
                    
                    if let Err(e) = config.validate() {
                        eprintln!("Validation error: {}", e);
                    }
                }
                ConfigType::App { name, version, debug } => {
                    let config = app!(
                        name,
                        version: version.as_deref().unwrap_or("1.0.0"),
                        debug: debug
                    );
                    
                    println!("Created application configuration:");
                    println!("{}", serde_json::to_string_pretty(&config).unwrap());
                    
                    if let Err(e) = config.validate() {
                        eprintln!("Validation error: {}", e);
                    }
                }
            }
        }
        Commands::Parse { input, validate } => {
            match DSLParser::parse_file(&input) {
                Ok(config) => {
                    println!("Parsed configuration:");
                    println!("{}", serde_json::to_string_pretty(&config).unwrap());
                    
                    if validate {
                        match config.validate() {
                            Ok(_) => println!("✓ Configuration is valid"),
                            Err(e) => eprintln!("✗ Validation error: {}", e),
                        }
                    }
                }
                Err(e) => {
                    eprintln!("Error parsing file: {}", e);
                }
            }
        }
        Commands::Generate { input, output } => {
            match DSLParser::parse_file(&input) {
                Ok(config) => {
                    let rust_code = CodeGenerator::generate_rust_struct(&config);
                    let json_schema = CodeGenerator::generate_json_schema(&config);
                    
                    if let Some(output_path) = output {
                        let mut content = String::new();
                        content.push_str("// Generated Rust struct\n");
                        content.push_str(&rust_code);
                        content.push_str("\n// Generated JSON Schema\n");
                        content.push_str(&json_schema);
                        
                        fs::write(&output_path, content).unwrap();
                        println!("Generated code saved to: {}", output_path);
                    } else {
                        println!("Generated Rust struct:");
                        println!("{}", rust_code);
                        println!("\nGenerated JSON Schema:");
                        println!("{}", json_schema);
                    }
                }
                Err(e) => {
                    eprintln!("Error generating code: {}", e);
                }
            }
        }
        Commands::Examples => {
            println!("DSL Examples:");
            println!();
            
            println!("1. Web Server Configuration:");
            let server_config = server!(port: 3000, host: "0.0.0.0", ssl: true);
            println!("{}", serde_json::to_string_pretty(&server_config).unwrap());
            println!();
            
            println!("2. Database Configuration:");
            let db_config = database!("localhost", 5432, "myapp", "admin");
            println!("{}", serde_json::to_string_pretty(&db_config).unwrap());
            println!();
            
            println!("3. Application Configuration:");
            let app_config = app!("MyApp", version: "2.1.0", debug: true);
            println!("{}", serde_json::to_string_pretty(&app_config).unwrap());
            println!();
            
            println!("4. Generated Rust Code:");
            let rust_code = CodeGenerator::generate_rust_struct(&server_config);
            println!("{}", rust_code);
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_config_creation() {
        let config = config!(test {
            name: "test",
            port: 8080,
            debug: true,
        });
        
        assert_eq!(config.name, "test");
        assert_eq!(config.get_string("name"), Some("test".to_string()));
        assert_eq!(config.get_number("port"), Some(8080));
        assert_eq!(config.get_boolean("debug"), Some(true));
    }

    #[test]
    fn test_server_macro() {
        let config = server!(port: 3000);
        assert_eq!(config.get_number("port"), Some(3000));
        assert_eq!(config.get_string("host"), Some("localhost".to_string()));
        assert_eq!(config.get_boolean("ssl"), Some(false));
    }

    #[test]
    fn test_database_macro() {
        let config = database!("localhost", 5432, "testdb", "user");
        assert_eq!(config.get_string("host"), Some("localhost".to_string()));
        assert_eq!(config.get_number("port"), Some(5432));
        assert_eq!(config.get_string("name"), Some("testdb".to_string()));
        assert_eq!(config.get_string("user"), Some("user".to_string()));
    }

    #[test]
    fn test_config_validation() {
        let mut config = Config::new("server".to_string());
        config.set("port", ConfigValue::Number(8080));
        config.set("host", ConfigValue::String("localhost".to_string()));
        config.set("ssl", ConfigValue::Boolean(false));
        
        assert!(config.validate().is_ok());
        
        config.fields.remove("port");
        assert!(config.validate().is_err());
    }

    #[test]
    fn test_dsl_parser() {
        let content = "port = 8080\nhost = \"localhost\"\ndebug = true";
        let config = DSLParser::parse_content(content).unwrap();
        
        assert_eq!(config.get_number("port"), Some(8080));
        assert_eq!(config.get_string("host"), Some("localhost".to_string()));
        assert_eq!(config.get_boolean("debug"), Some(true));
    }
}