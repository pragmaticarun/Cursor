use clap::{Parser, Subcommand};
use serde::{Deserialize, Serialize};
use std::collections::HashMap;
use std::fs;
use std::io;

#[derive(Parser)]
#[command(name = "library-management")]
#[command(about = "A library management system demonstrating structs, enums, and pattern matching")]
struct Cli {
    #[command(subcommand)]
    command: Commands,
}

#[derive(Subcommand)]
enum Commands {
    /// Add a new book to the library
    Add {
        title: String,
        author: String,
        isbn: String,
        #[arg(short, long, value_parser = clap::value_parser!(String))]
        book_type: String,
    },
    /// Remove a book from the library
    Remove {
        isbn: String,
    },
    /// List all books
    List,
    /// Search for books
    Search {
        query: String,
    },
    /// Borrow a book
    Borrow {
        isbn: String,
        borrower: String,
    },
    /// Return a book
    Return {
        isbn: String,
    },
    /// Show library statistics
    Stats,
}

#[derive(Debug, Clone, Serialize, Deserialize, PartialEq, Eq, Hash)]
enum BookType {
    Fiction,
    NonFiction,
    Reference,
    Textbook,
    Magazine,
}

impl std::fmt::Display for BookType {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        match self {
            BookType::Fiction => write!(f, "Fiction"),
            BookType::NonFiction => write!(f, "Non-Fiction"),
            BookType::Reference => write!(f, "Reference"),
            BookType::Textbook => write!(f, "Textbook"),
            BookType::Magazine => write!(f, "Magazine"),
        }
    }
}

#[derive(Debug, Clone, Serialize, Deserialize)]
enum BookStatus {
    Available,
    Borrowed {
        borrower: String,
        borrow_date: chrono::NaiveDate,
    },
    Reserved {
        borrower: String,
        reserve_date: chrono::NaiveDate,
    },
}

impl BookStatus {
    fn is_available(&self) -> bool {
        matches!(self, BookStatus::Available)
    }
    
    fn get_borrower(&self) -> Option<&String> {
        match self {
            BookStatus::Borrowed { borrower, .. } | BookStatus::Reserved { borrower, .. } => {
                Some(borrower)
            }
            BookStatus::Available => None,
        }
    }
}

#[derive(Debug, Clone, Serialize, Deserialize)]
struct Book {
    title: String,
    author: String,
    isbn: String,
    book_type: BookType,
    status: BookStatus,
    added_date: chrono::NaiveDate,
}

impl Book {
    fn new(title: String, author: String, isbn: String, book_type: BookType) -> Self {
        Self {
            title,
            author,
            isbn,
            book_type,
            status: BookStatus::Available,
            added_date: chrono::Local::now().date_naive(),
        }
    }
    
    fn borrow(&mut self, borrower: String) -> Result<(), String> {
        match &self.status {
            BookStatus::Available => {
                self.status = BookStatus::Borrowed {
                    borrower,
                    borrow_date: chrono::Local::now().date_naive(),
                };
                Ok(())
            }
            BookStatus::Borrowed { borrower, .. } => {
                Err(format!("Book is already borrowed by {}", borrower))
            }
            BookStatus::Reserved { borrower, .. } => {
                Err(format!("Book is reserved by {}", borrower))
            }
        }
    }
    
    fn return_book(&mut self) -> Result<(), String> {
        match &self.status {
            BookStatus::Borrowed { .. } => {
                self.status = BookStatus::Available;
                Ok(())
            }
            BookStatus::Available => Err("Book is not borrowed".to_string()),
            BookStatus::Reserved { .. } => Err("Book is reserved, not borrowed".to_string()),
        }
    }
    
    fn reserve(&mut self, borrower: String) -> Result<(), String> {
        match &self.status {
            BookStatus::Available => {
                self.status = BookStatus::Reserved {
                    borrower,
                    reserve_date: chrono::Local::now().date_naive(),
                };
                Ok(())
            }
            BookStatus::Borrowed { borrower, .. } => {
                Err(format!("Book is borrowed by {}", borrower))
            }
            BookStatus::Reserved { borrower, .. } => {
                Err(format!("Book is already reserved by {}", borrower))
            }
        }
    }
}

#[derive(Debug, Serialize, Deserialize)]
struct Library {
    books: HashMap<String, Book>,
}

impl Library {
    fn new() -> Self {
        Self {
            books: HashMap::new(),
        }
    }
    
    fn add_book(&mut self, book: Book) -> Result<(), String> {
        if self.books.contains_key(&book.isbn) {
            return Err("Book with this ISBN already exists".to_string());
        }
        self.books.insert(book.isbn.clone(), book);
        Ok(())
    }
    
    fn remove_book(&mut self, isbn: &str) -> Result<Book, String> {
        self.books.remove(isbn).ok_or_else(|| "Book not found".to_string())
    }
    
    fn get_book(&mut self, isbn: &str) -> Result<&mut Book, String> {
        self.books.get_mut(isbn).ok_or_else(|| "Book not found".to_string())
    }
    
    fn search_books(&self, query: &str) -> Vec<&Book> {
        let query_lower = query.to_lowercase();
        self.books
            .values()
            .filter(|book| {
                book.title.to_lowercase().contains(&query_lower)
                    || book.author.to_lowercase().contains(&query_lower)
                    || book.isbn.contains(&query_lower)
            })
            .collect()
    }
    
    fn get_stats(&self) -> LibraryStats {
        let total_books = self.books.len();
        let available_books = self.books.values().filter(|b| b.status.is_available()).count();
        let borrowed_books = self.books.values().filter(|b| matches!(b.status, BookStatus::Borrowed { .. })).count();
        let reserved_books = self.books.values().filter(|b| matches!(b.status, BookStatus::Reserved { .. })).count();
        
        let mut type_counts = HashMap::new();
        for book in self.books.values() {
            let count = type_counts.entry(book.book_type.to_string()).or_insert(0);
            *count += 1;
        }
        
        LibraryStats {
            total_books,
            available_books,
            borrowed_books,
            reserved_books,
            type_counts,
        }
    }
    
    fn save_to_file(&self, filename: &str) -> Result<(), String> {
        let json = serde_json::to_string_pretty(self)
            .map_err(|e| format!("Failed to serialize library: {}", e))?;
        fs::write(filename, json)
            .map_err(|e| format!("Failed to write file: {}", e))?;
        Ok(())
    }
    
    fn load_from_file(filename: &str) -> Result<Self, String> {
        let content = fs::read_to_string(filename)
            .map_err(|e| format!("Failed to read file: {}", e))?;
        let library: Library = serde_json::from_str(&content)
            .map_err(|e| format!("Failed to parse library: {}", e))?;
        Ok(library)
    }
}

#[derive(Debug)]
struct LibraryStats {
    total_books: usize,
    available_books: usize,
    borrowed_books: usize,
    reserved_books: usize,
    type_counts: HashMap<String, usize>,
}

impl std::fmt::Display for LibraryStats {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        writeln!(f, "Library Statistics:")?;
        writeln!(f, "  Total books: {}", self.total_books)?;
        writeln!(f, "  Available: {}", self.available_books)?;
        writeln!(f, "  Borrowed: {}", self.borrowed_books)?;
        writeln!(f, "  Reserved: {}", self.reserved_books)?;
        writeln!(f, "  Books by type:")?;
        for (book_type, count) in &self.type_counts {
            writeln!(f, "    {}: {}", book_type, count)?;
        }
        Ok(())
    }
}

fn main() {
    let cli = Cli::parse();
    
    // Try to load existing library, or create new one
    let mut library = Library::load_from_file("library.json").unwrap_or_else(|_| {
        println!("Creating new library...");
        Library::new()
    });
    
    match cli.command {
        Commands::Add {
            title,
            author,
            isbn,
            book_type,
        } => {
            let book_type_enum = match book_type.to_lowercase().as_str() {
                "fiction" => BookType::Fiction,
                "nonfiction" | "non-fiction" => BookType::NonFiction,
                "reference" => BookType::Reference,
                "textbook" => BookType::Textbook,
                "magazine" => BookType::Magazine,
                _ => {
                    eprintln!("Invalid book type: {}. Valid types: fiction, nonfiction, reference, textbook, magazine", book_type);
                    return;
                }
            };
            let book = Book::new(title, author, isbn.clone(), book_type_enum);
            match library.add_book(book) {
                Ok(_) => println!("Book '{}' added successfully!", isbn),
                Err(e) => println!("Error: {}", e),
            }
        }
        Commands::Remove { isbn } => {
            match library.remove_book(&isbn) {
                Ok(book) => println!("Book '{}' by {} removed successfully!", book.title, book.author),
                Err(e) => println!("Error: {}", e),
            }
        }
        Commands::List => {
            if library.books.is_empty() {
                println!("No books in the library.");
            } else {
                println!("Books in the library:");
                for book in library.books.values() {
                    println!(
                        "  {} by {} (ISBN: {}) - {} - Status: {:?}",
                        book.title, book.author, book.isbn, book.book_type, book.status
                    );
                }
            }
        }
        Commands::Search { query } => {
            let results = library.search_books(&query);
            if results.is_empty() {
                println!("No books found matching '{}'", query);
            } else {
                println!("Search results for '{}':", query);
                for book in results {
                    println!(
                        "  {} by {} (ISBN: {}) - {} - Status: {:?}",
                        book.title, book.author, book.isbn, book.book_type, book.status
                    );
                }
            }
        }
        Commands::Borrow { isbn, borrower } => {
            match library.get_book(&isbn) {
                Ok(book) => {
                    match book.borrow(borrower) {
                        Ok(_) => println!("Book '{}' borrowed successfully!", book.title),
                        Err(e) => println!("Error: {}", e),
                    }
                }
                Err(e) => println!("Error: {}", e),
            }
        }
        Commands::Return { isbn } => {
            match library.get_book(&isbn) {
                Ok(book) => {
                    match book.return_book() {
                        Ok(_) => println!("Book '{}' returned successfully!", book.title),
                        Err(e) => println!("Error: {}", e),
                    }
                }
                Err(e) => println!("Error: {}", e),
            }
        }
        Commands::Stats => {
            println!("{}", library.get_stats());
        }
    }
    
    // Save library after each operation
    if let Err(e) = library.save_to_file("library.json") {
        eprintln!("Warning: Failed to save library: {}", e);
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_book_creation() {
        let book = Book::new(
            "The Rust Book".to_string(),
            "Steve Klabnik".to_string(),
            "978-1492073106".to_string(),
            BookType::Textbook,
        );
        assert_eq!(book.title, "The Rust Book");
        assert_eq!(book.author, "Steve Klabnik");
        assert_eq!(book.isbn, "978-1492073106");
        assert!(book.status.is_available());
    }

    #[test]
    fn test_book_borrow() {
        let mut book = Book::new(
            "Test Book".to_string(),
            "Test Author".to_string(),
            "1234567890".to_string(),
            BookType::Fiction,
        );
        
        assert!(book.borrow("John Doe".to_string()).is_ok());
        assert!(book.status.get_borrower().is_some());
        
        // Try to borrow again
        assert!(book.borrow("Jane Doe".to_string()).is_err());
    }

    #[test]
    fn test_library_operations() {
        let mut library = Library::new();
        let book = Book::new(
            "Test Book".to_string(),
            "Test Author".to_string(),
            "1234567890".to_string(),
            BookType::Fiction,
        );
        
        assert!(library.add_book(book).is_ok());
        assert_eq!(library.books.len(), 1);
        
        let results = library.search_books("Test");
        assert_eq!(results.len(), 1);
    }
}