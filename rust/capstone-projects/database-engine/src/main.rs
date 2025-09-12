use clap::{Parser, Subcommand};
use dashmap::DashMap;
use serde::{Deserialize, Serialize};
use std::collections::{BTreeMap, HashMap};
use std::fs;
use std::io::{self, Write};
use std::sync::Arc;
use thiserror::Error;
use uuid::Uuid;

#[derive(Parser)]
#[command(name = "database-engine")]
#[command(about = "A simple database engine with B-tree indexing and transactions")]
struct Cli {
    #[command(subcommand)]
    command: Commands,
}

#[derive(Subcommand)]
enum Commands {
    /// Start the database server
    Start {
        #[arg(short, long, default_value = "data")]
        data_dir: String,
        #[arg(short, long, default_value = "8080")]
        port: u16,
    },
    /// Execute SQL commands
    Query {
        sql: String,
        #[arg(short, long, default_value = "data")]
        data_dir: String,
    },
    /// Create a new table
    CreateTable {
        name: String,
        schema: String,
        #[arg(short, long, default_value = "data")]
        data_dir: String,
    },
    /// Insert data into a table
    Insert {
        table: String,
        data: String,
        #[arg(short, long, default_value = "data")]
        data_dir: String,
    },
    /// Select data from a table
    Select {
        table: String,
        #[arg(short, long)]
        where_clause: Option<String>,
        #[arg(short, long, default_value = "data")]
        data_dir: String,
    },
    /// Show database statistics
    Stats {
        #[arg(short, long, default_value = "data")]
        data_dir: String,
    },
}

#[derive(Error, Debug)]
enum DatabaseError {
    #[error("IO error: {0}")]
    IoError(#[from] io::Error),
    #[error("Serialization error: {0}")]
    SerializationError(#[from] bincode::Error),
    #[error("JSON error: {0}")]
    JsonError(#[from] serde_json::Error),
    #[error("Table not found: {0}")]
    TableNotFound(String),
    #[error("Column not found: {0}")]
    ColumnNotFound(String),
    #[error("Invalid SQL: {0}")]
    InvalidSql(String),
    #[error("Transaction error: {0}")]
    TransactionError(String),
    #[error("Constraint violation: {0}")]
    ConstraintViolation(String),
}

#[derive(Debug, Clone, Serialize, Deserialize)]
struct Column {
    name: String,
    data_type: DataType,
    nullable: bool,
    primary_key: bool,
    unique: bool,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
enum DataType {
    Integer,
    Text,
    Boolean,
    Float,
    Timestamp,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
struct TableSchema {
    name: String,
    columns: Vec<Column>,
    indexes: Vec<Index>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
struct Index {
    name: String,
    columns: Vec<String>,
    unique: bool,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
struct Row {
    id: Uuid,
    data: HashMap<String, Value>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
enum Value {
    Integer(i64),
    Text(String),
    Boolean(bool),
    Float(f64),
    Timestamp(chrono::DateTime<chrono::Utc>),
    Null,
}

impl Value {
    fn matches_type(&self, data_type: &DataType) -> bool {
        match (self, data_type) {
            (Value::Integer(_), DataType::Integer) => true,
            (Value::Text(_), DataType::Text) => true,
            (Value::Boolean(_), DataType::Boolean) => true,
            (Value::Float(_), DataType::Float) => true,
            (Value::Timestamp(_), DataType::Timestamp) => true,
            (Value::Null, _) => true,
            _ => false,
        }
    }
}

#[derive(Debug, Clone, Serialize, Deserialize)]
struct Table {
    schema: TableSchema,
    rows: BTreeMap<Uuid, Row>,
    indexes: HashMap<String, BTreeMap<Value, Vec<Uuid>>>,
}

impl Table {
    fn new(schema: TableSchema) -> Self {
        Self {
            schema,
            rows: BTreeMap::new(),
            indexes: HashMap::new(),
        }
    }
    
    fn insert(&mut self, data: HashMap<String, Value>) -> Result<Uuid, DatabaseError> {
        // Validate data against schema
        for column in &self.schema.columns {
            if let Some(value) = data.get(&column.name) {
                if !value.matches_type(&column.data_type) {
                    return Err(DatabaseError::ConstraintViolation(
                        format!("Column {} expects {:?}, got {:?}", column.name, column.data_type, value)
                    ));
                }
                
                if !column.nullable && matches!(value, Value::Null) {
                    return Err(DatabaseError::ConstraintViolation(
                        format!("Column {} cannot be null", column.name)
                    ));
                }
            } else if !column.nullable {
                return Err(DatabaseError::ConstraintViolation(
                    format!("Column {} is required", column.name)
                ));
            }
        }
        
        // Check unique constraints
        for column in &self.schema.columns {
            if column.unique {
                if let Some(value) = data.get(&column.name) {
                    if let Some(index) = self.indexes.get(&column.name) {
                        if index.contains_key(value) {
                            return Err(DatabaseError::ConstraintViolation(
                                format!("Duplicate value for unique column {}", column.name)
                            ));
                        }
                    }
                }
            }
        }
        
        let id = Uuid::new_v4();
        let row = Row { id, data };
        
        // Update indexes
        for column in &self.schema.columns {
            if let Some(value) = row.data.get(&column.name) {
                self.indexes.entry(column.name.clone())
                    .or_insert_with(BTreeMap::new)
                    .entry(value.clone())
                    .or_insert_with(Vec::new)
                    .push(id);
            }
        }
        
        self.rows.insert(id, row);
        Ok(id)
    }
    
    fn select(&self, where_clause: Option<&str>) -> Result<Vec<Row>, DatabaseError> {
        let mut results = Vec::new();
        
        for row in self.rows.values() {
            if let Some(condition) = where_clause {
                if !self.evaluate_condition(row, condition)? {
                    continue;
                }
            }
            results.push(row.clone());
        }
        
        Ok(results)
    }
    
    fn evaluate_condition(&self, row: &Row, condition: &str) -> Result<bool, DatabaseError> {
        // Simple condition evaluation (e.g., "age > 18")
        let parts: Vec<&str> = condition.split_whitespace().collect();
        if parts.len() != 3 {
            return Err(DatabaseError::InvalidSql("Invalid condition format".to_string()));
        }
        
        let column_name = parts[0];
        let operator = parts[1];
        let value_str = parts[2];
        
        let column_value = row.data.get(column_name)
            .ok_or_else(|| DatabaseError::ColumnNotFound(column_name.to_string()))?;
        
        // Parse the comparison value based on column type
        let column = self.schema.columns.iter()
            .find(|c| c.name == column_name)
            .ok_or_else(|| DatabaseError::ColumnNotFound(column_name.to_string()))?;
        
        let comparison_value = match column.data_type {
            DataType::Integer => Value::Integer(value_str.parse().map_err(|_| {
                DatabaseError::InvalidSql(format!("Invalid integer: {}", value_str))
            })?),
            DataType::Text => Value::Text(value_str.to_string()),
            DataType::Boolean => Value::Boolean(value_str.parse().map_err(|_| {
                DatabaseError::InvalidSql(format!("Invalid boolean: {}", value_str))
            })?),
            DataType::Float => Value::Float(value_str.parse().map_err(|_| {
                DatabaseError::InvalidSql(format!("Invalid float: {}", value_str))
            })?),
            DataType::Timestamp => Value::Timestamp(chrono::DateTime::parse_from_rfc3339(value_str)
                .map_err(|_| DatabaseError::InvalidSql(format!("Invalid timestamp: {}", value_str)))?
                .with_timezone(&chrono::Utc)),
        };
        
        let result = match operator {
            "=" => column_value == &comparison_value,
            "!=" => column_value != &comparison_value,
            ">" => self.compare_values(column_value, &comparison_value)? > 0,
            "<" => self.compare_values(column_value, &comparison_value)? < 0,
            ">=" => self.compare_values(column_value, &comparison_value)? >= 0,
            "<=" => self.compare_values(column_value, &comparison_value)? <= 0,
            _ => return Err(DatabaseError::InvalidSql(format!("Unknown operator: {}", operator))),
        };
        
        Ok(result)
    }
    
    fn compare_values(&self, a: &Value, b: &Value) -> Result<i32, DatabaseError> {
        match (a, b) {
            (Value::Integer(x), Value::Integer(y)) => Ok((x - y).signum() as i32),
            (Value::Text(x), Value::Text(y)) => Ok(x.cmp(y) as i32),
            (Value::Boolean(x), Value::Boolean(y)) => Ok((*x as i32) - (*y as i32)),
            (Value::Float(x), Value::Float(y)) => Ok((x - y).signum() as i32),
            (Value::Timestamp(x), Value::Timestamp(y)) => Ok(x.cmp(y) as i32),
            _ => Err(DatabaseError::InvalidSql("Cannot compare different types".to_string())),
        }
    }
}

#[derive(Debug, Clone, Serialize, Deserialize)]
struct Transaction {
    id: Uuid,
    operations: Vec<TransactionOperation>,
    status: TransactionStatus,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
enum TransactionOperation {
    Insert { table: String, data: HashMap<String, Value> },
    Update { table: String, id: Uuid, data: HashMap<String, Value> },
    Delete { table: String, id: Uuid },
}

#[derive(Debug, Clone, Serialize, Deserialize)]
enum TransactionStatus {
    Active,
    Committed,
    RolledBack,
}

struct Database {
    tables: DashMap<String, Table>,
    transactions: DashMap<Uuid, Transaction>,
    data_dir: String,
}

impl Database {
    fn new(data_dir: String) -> Self {
        Self {
            tables: DashMap::new(),
            transactions: DashMap::new(),
            data_dir,
        }
    }
    
    fn create_table(&self, name: String, schema: TableSchema) -> Result<(), DatabaseError> {
        if self.tables.contains_key(&name) {
            return Err(DatabaseError::ConstraintViolation(
                format!("Table {} already exists", name)
            ));
        }
        
        let table = Table::new(schema);
        self.tables.insert(name, table);
        self.save_table(&name)?;
        Ok(())
    }
    
    fn get_table(&self, name: &str) -> Result<Arc<Table>, DatabaseError> {
        self.tables.get(name)
            .ok_or_else(|| DatabaseError::TableNotFound(name.to_string()))
            .map(|entry| Arc::new(entry.clone()))
    }
    
    fn insert(&self, table_name: &str, data: HashMap<String, Value>) -> Result<Uuid, DatabaseError> {
        let mut table = self.tables.get_mut(table_name)
            .ok_or_else(|| DatabaseError::TableNotFound(table_name.to_string()))?;
        
        let id = table.insert(data)?;
        self.save_table(table_name)?;
        Ok(id)
    }
    
    fn select(&self, table_name: &str, where_clause: Option<&str>) -> Result<Vec<Row>, DatabaseError> {
        let table = self.get_table(table_name)?;
        table.select(where_clause)
    }
    
    fn save_table(&self, table_name: &str) -> Result<(), DatabaseError> {
        let table = self.tables.get(table_name)
            .ok_or_else(|| DatabaseError::TableNotFound(table_name.to_string()))?;
        
        let path = format!("{}/{}.bin", self.data_dir, table_name);
        let serialized = bincode::serialize(&*table)?;
        fs::write(path, serialized)?;
        Ok(())
    }
    
    fn load_table(&self, table_name: &str) -> Result<(), DatabaseError> {
        let path = format!("{}/{}.bin", self.data_dir, table_name);
        
        if !fs::metadata(&path).is_ok() {
            return Ok(()); // Table doesn't exist yet
        }
        
        let data = fs::read(&path)?;
        let table: Table = bincode::deserialize(&data)?;
        self.tables.insert(table_name.to_string(), table);
        Ok(())
    }
    
    fn load_all_tables(&self) -> Result<(), DatabaseError> {
        if !fs::metadata(&self.data_dir).is_ok() {
            fs::create_dir_all(&self.data_dir)?;
            return Ok(());
        }
        
        let entries = fs::read_dir(&self.data_dir)?;
        for entry in entries {
            let entry = entry?;
            let path = entry.path();
            
            if path.extension().and_then(|s| s.to_str()) == Some("bin") {
                if let Some(table_name) = path.file_stem().and_then(|s| s.to_str()) {
                    self.load_table(table_name)?;
                }
            }
        }
        
        Ok(())
    }
    
    fn get_stats(&self) -> DatabaseStats {
        let mut total_rows = 0;
        let mut total_size = 0;
        
        for table in self.tables.iter() {
            total_rows += table.rows.len();
            total_size += std::mem::size_of_val(&*table);
        }
        
        DatabaseStats {
            table_count: self.tables.len(),
            total_rows,
            total_size,
            active_transactions: self.transactions.len(),
        }
    }
}

#[derive(Debug, Serialize, Deserialize)]
struct DatabaseStats {
    table_count: usize,
    total_rows: usize,
    total_size: usize,
    active_transactions: usize,
}

fn parse_schema(schema_str: &str) -> Result<TableSchema, DatabaseError> {
    let schema_data: serde_json::Value = serde_json::from_str(schema_str)?;
    
    let name = schema_data["name"].as_str()
        .ok_or_else(|| DatabaseError::InvalidSql("Missing table name".to_string()))?
        .to_string();
    
    let columns_data = schema_data["columns"].as_array()
        .ok_or_else(|| DatabaseError::InvalidSql("Missing columns".to_string()))?;
    
    let mut columns = Vec::new();
    for col_data in columns_data {
        let name = col_data["name"].as_str()
            .ok_or_else(|| DatabaseError::InvalidSql("Missing column name".to_string()))?
            .to_string();
        
        let data_type_str = col_data["type"].as_str()
            .ok_or_else(|| DatabaseError::InvalidSql("Missing column type".to_string()))?;
        
        let data_type = match data_type_str {
            "integer" => DataType::Integer,
            "text" => DataType::Text,
            "boolean" => DataType::Boolean,
            "float" => DataType::Float,
            "timestamp" => DataType::Timestamp,
            _ => return Err(DatabaseError::InvalidSql(format!("Unknown type: {}", data_type_str))),
        };
        
        let nullable = col_data["nullable"].as_bool().unwrap_or(true);
        let primary_key = col_data["primary_key"].as_bool().unwrap_or(false);
        let unique = col_data["unique"].as_bool().unwrap_or(false);
        
        columns.push(Column {
            name,
            data_type,
            nullable,
            primary_key,
            unique,
        });
    }
    
    Ok(TableSchema {
        name,
        columns,
        indexes: Vec::new(),
    })
}

fn parse_data(data_str: &str) -> Result<HashMap<String, Value>, DatabaseError> {
    let data_json: serde_json::Value = serde_json::from_str(data_str)?;
    let mut result = HashMap::new();
    
    for (key, value) in data_json.as_object().unwrap() {
        let parsed_value = match value {
            serde_json::Value::Number(n) => {
                if n.is_i64() {
                    Value::Integer(n.as_i64().unwrap())
                } else {
                    Value::Float(n.as_f64().unwrap())
                }
            }
            serde_json::Value::String(s) => Value::Text(s.clone()),
            serde_json::Value::Bool(b) => Value::Boolean(*b),
            serde_json::Value::Null => Value::Null,
            _ => return Err(DatabaseError::InvalidSql("Unsupported value type".to_string())),
        };
        result.insert(key.clone(), parsed_value);
    }
    
    Ok(result)
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let cli = Cli::parse();
    
    match cli.command {
        Commands::Start { data_dir, port } => {
            println!("Starting database server on port {} with data directory: {}", port, data_dir);
            
            let db = Arc::new(Database::new(data_dir));
            db.load_all_tables()?;
            
            println!("Database loaded successfully");
            println!("Tables: {}", db.tables.len());
            
            // In a real implementation, you would start an HTTP server here
            println!("Server would be running on port {}", port);
            println!("Press Ctrl+C to stop");
            
            // Keep the server running
            loop {
                std::thread::sleep(std::time::Duration::from_secs(1));
            }
        }
        Commands::Query { sql, data_dir } => {
            let db = Arc::new(Database::new(data_dir));
            db.load_all_tables()?;
            
            println!("Executing SQL: {}", sql);
            println!("(SQL parsing not implemented in this example)");
        }
        Commands::CreateTable { name, schema, data_dir } => {
            let db = Arc::new(Database::new(data_dir));
            db.load_all_tables()?;
            
            let table_schema = parse_schema(&schema)?;
            db.create_table(name.clone(), table_schema)?;
            
            println!("Table '{}' created successfully", name);
        }
        Commands::Insert { table, data, data_dir } => {
            let db = Arc::new(Database::new(data_dir));
            db.load_all_tables()?;
            
            let row_data = parse_data(&data)?;
            let id = db.insert(&table, row_data)?;
            
            println!("Inserted row with ID: {}", id);
        }
        Commands::Select { table, where_clause, data_dir } => {
            let db = Arc::new(Database::new(data_dir));
            db.load_all_tables()?;
            
            let rows = db.select(&table, where_clause.as_deref())?;
            
            println!("Found {} rows in table '{}'", rows.len(), table);
            for (i, row) in rows.iter().enumerate() {
                println!("Row {}: ID={}, Data={:?}", i + 1, row.id, row.data);
            }
        }
        Commands::Stats { data_dir } => {
            let db = Arc::new(Database::new(data_dir));
            db.load_all_tables()?;
            
            let stats = db.get_stats();
            println!("Database Statistics:");
            println!("  Tables: {}", stats.table_count);
            println!("  Total rows: {}", stats.total_rows);
            println!("  Memory usage: {} bytes", stats.total_size);
            println!("  Active transactions: {}", stats.active_transactions);
        }
    }
    
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_table_creation() {
        let schema = TableSchema {
            name: "users".to_string(),
            columns: vec![
                Column {
                    name: "id".to_string(),
                    data_type: DataType::Integer,
                    nullable: false,
                    primary_key: true,
                    unique: true,
                },
                Column {
                    name: "name".to_string(),
                    data_type: DataType::Text,
                    nullable: false,
                    primary_key: false,
                    unique: false,
                },
            ],
            indexes: Vec::new(),
        };
        
        let table = Table::new(schema);
        assert_eq!(table.schema.name, "users");
        assert_eq!(table.schema.columns.len(), 2);
    }

    #[test]
    fn test_row_insertion() {
        let schema = TableSchema {
            name: "test".to_string(),
            columns: vec![
                Column {
                    name: "id".to_string(),
                    data_type: DataType::Integer,
                    nullable: false,
                    primary_key: false,
                    unique: false,
                },
            ],
            indexes: Vec::new(),
        };
        
        let mut table = Table::new(schema);
        let mut data = HashMap::new();
        data.insert("id".to_string(), Value::Integer(1));
        
        let result = table.insert(data);
        assert!(result.is_ok());
        assert_eq!(table.rows.len(), 1);
    }

    #[test]
    fn test_data_parsing() {
        let json = r#"{"id": 1, "name": "test", "active": true}"#;
        let data = parse_data(json).unwrap();
        
        assert_eq!(data.len(), 3);
        assert_eq!(data["id"], Value::Integer(1));
        assert_eq!(data["name"], Value::Text("test".to_string()));
        assert_eq!(data["active"], Value::Boolean(true));
    }
}