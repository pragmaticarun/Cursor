use clap::{Parser, Subcommand};
use futures::future::join_all;
use reqwest::Client;
use scraper::{Html, Selector};
use serde::{Deserialize, Serialize};
use std::collections::HashSet;
use std::sync::Arc;
use std::time::Duration;
use tokio::sync::Semaphore;
use tokio::time::timeout;
use url::{Url, ParseError};
use thiserror::Error;

#[derive(Parser)]
#[command(name = "web-scraper")]
#[command(about = "A concurrent web scraper demonstrating async/await and parallelism")]
struct Cli {
    #[command(subcommand)]
    command: Commands,
}

#[derive(Subcommand)]
enum Commands {
    /// Scrape a single URL
    Scrape {
        url: String,
        #[arg(short, long)]
        selector: Option<String>,
        #[arg(short, long)]
        output: Option<String>,
    },
    /// Scrape multiple URLs concurrently
    Batch {
        urls: Vec<String>,
        #[arg(short, long)]
        selector: Option<String>,
        #[arg(short, long)]
        max_concurrent: Option<usize>,
        #[arg(short, long)]
        output: Option<String>,
    },
    /// Crawl a website starting from a base URL
    Crawl {
        base_url: String,
        #[arg(short, long)]
        max_depth: Option<usize>,
        #[arg(short, long)]
        max_pages: Option<usize>,
        #[arg(short, long)]
        selector: Option<String>,
        #[arg(short, long)]
        output: Option<String>,
    },
    /// Extract specific data from URLs
    Extract {
        urls: Vec<String>,
        #[arg(short, long)]
        title: bool,
        #[arg(short, long)]
        links: bool,
        #[arg(short, long)]
        images: bool,
        #[arg(short, long)]
        text: bool,
        #[arg(short, long)]
        output: Option<String>,
    },
}

#[derive(Error, Debug)]
enum ScraperError {
    #[error("HTTP error: {0}")]
    HttpError(#[from] reqwest::Error),
    #[error("URL parse error: {0}")]
    UrlError(#[from] ParseError),
    #[error("Selector error: {0}")]
    SelectorError(String),
    #[error("Timeout error")]
    TimeoutError,
    #[error("IO error: {0}")]
    IoError(#[from] std::io::Error),
}

#[derive(Debug, Clone, Serialize, Deserialize)]
struct ScrapedData {
    url: String,
    title: Option<String>,
    text: Option<String>,
    links: Vec<String>,
    images: Vec<String>,
    timestamp: chrono::DateTime<chrono::Utc>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
struct CrawlResult {
    base_url: String,
    pages_scraped: usize,
    total_links_found: usize,
    data: Vec<ScrapedData>,
}

struct WebScraper {
    client: Client,
    semaphore: Arc<Semaphore>,
}

impl WebScraper {
    fn new(max_concurrent: usize) -> Self {
        let client = Client::builder()
            .timeout(Duration::from_secs(30))
            .user_agent("Mozilla/5.0 (compatible; Rust Web Scraper)")
            .build()
            .expect("Failed to create HTTP client");
        
        Self {
            client,
            semaphore: Arc::new(Semaphore::new(max_concurrent)),
        }
    }
    
    async fn scrape_url(&self, url: &str, selector: Option<&str>) -> Result<ScrapedData, ScraperError> {
        let _permit = self.semaphore.acquire().await.map_err(|_| ScraperError::TimeoutError)?;
        
        let response = timeout(Duration::from_secs(30), self.client.get(url).send())
            .await
            .map_err(|_| ScraperError::TimeoutError)?
            .map_err(ScraperError::HttpError)?;
        
        if !response.status().is_success() {
            return Err(ScraperError::HttpError(
                reqwest::Error::from(response.error_for_status().unwrap_err())
            ));
        }
        
        let html = response.text().await.map_err(ScraperError::HttpError)?;
        let document = Html::parse_document(&html);
        
        let title = document
            .select(&Selector::parse("title").map_err(|e| ScraperError::SelectorError(e.to_string()))?)
            .next()
            .map(|title| title.text().collect::<String>().trim().to_string());
        
        let text = if let Some(selector_str) = selector {
            let selector = Selector::parse(selector_str)
                .map_err(|e| ScraperError::SelectorError(e.to_string()))?;
            Some(document.select(&selector).map(|el| el.text().collect::<String>()).collect::<Vec<_>>().join(" "))
        } else {
            Some(document.select(&Selector::parse("body").unwrap()).map(|el| el.text().collect::<String>()).collect::<Vec<_>>().join(" "))
        };
        
        let links: Vec<String> = document
            .select(&Selector::parse("a[href]").unwrap())
            .filter_map(|el| el.value().attr("href"))
            .filter_map(|href| {
                if href.starts_with("http") {
                    Some(href.to_string())
                } else if href.starts_with("/") {
                    Url::parse(url).ok().and_then(|base| base.join(href).ok()).map(|u| u.to_string())
                } else {
                    None
                }
            })
            .collect();
        
        let images: Vec<String> = document
            .select(&Selector::parse("img[src]").unwrap())
            .filter_map(|el| el.value().attr("src"))
            .filter_map(|src| {
                if src.starts_with("http") {
                    Some(src.to_string())
                } else if src.starts_with("/") {
                    Url::parse(url).ok().and_then(|base| base.join(src).ok()).map(|u| u.to_string())
                } else {
                    None
                }
            })
            .collect();
        
        Ok(ScrapedData {
            url: url.to_string(),
            title,
            text,
            links,
            images,
            timestamp: chrono::Utc::now(),
        })
    }
    
    async fn scrape_multiple(&self, urls: Vec<String>, selector: Option<&str>) -> Result<Vec<ScrapedData>, ScraperError> {
        let tasks: Vec<_> = urls.into_iter()
            .map(|url| {
                let scraper = self.clone();
                tokio::spawn(async move {
                    scraper.scrape_url(&url, selector).await
                })
            })
            .collect();
        
        let results = join_all(tasks).await;
        let mut scraped_data = Vec::new();
        
        for result in results {
            match result {
                Ok(Ok(data)) => scraped_data.push(data),
                Ok(Err(e)) => eprintln!("Error scraping URL: {}", e),
                Err(e) => eprintln!("Task error: {}", e),
            }
        }
        
        Ok(scraped_data)
    }
    
    async fn crawl_website(&self, base_url: &str, max_depth: usize, max_pages: usize, selector: Option<&str>) -> Result<CrawlResult, ScraperError> {
        let mut visited = HashSet::new();
        let mut to_visit = vec![(base_url.to_string(), 0)];
        let mut scraped_data = Vec::new();
        let mut total_links = 0;
        
        while !to_visit.is_empty() && scraped_data.len() < max_pages {
            let (url, depth) = to_visit.pop().unwrap();
            
            if visited.contains(&url) || depth > max_depth {
                continue;
            }
            
            visited.insert(url.clone());
            
            match self.scrape_url(&url, selector).await {
                Ok(data) => {
                    total_links += data.links.len();
                    
                    // Add new links to visit queue
                    for link in &data.links {
                        if !visited.contains(link) && link.starts_with(base_url) {
                            to_visit.push((link.clone(), depth + 1));
                        }
                    }
                    
                    scraped_data.push(data);
                    println!("Scraped: {} (depth: {})", url, depth);
                }
                Err(e) => {
                    eprintln!("Error scraping {}: {}", url, e);
                }
            }
        }
        
        Ok(CrawlResult {
            base_url: base_url.to_string(),
            pages_scraped: scraped_data.len(),
            total_links_found: total_links,
            data: scraped_data,
        })
    }
    
    async fn extract_specific_data(&self, urls: Vec<String>, extract_title: bool, extract_links: bool, extract_images: bool, extract_text: bool) -> Result<Vec<ScrapedData>, ScraperError> {
        let mut results = Vec::new();
        
        for url in urls {
            match self.scrape_url(&url, None).await {
                Ok(mut data) => {
                    if !extract_title {
                        data.title = None;
                    }
                    if !extract_text {
                        data.text = None;
                    }
                    if !extract_links {
                        data.links.clear();
                    }
                    if !extract_images {
                        data.images.clear();
                    }
                    results.push(data);
                }
                Err(e) => {
                    eprintln!("Error extracting data from {}: {}", url, e);
                }
            }
        }
        
        Ok(results)
    }
}

impl Clone for WebScraper {
    fn clone(&self) -> Self {
        Self {
            client: self.client.clone(),
            semaphore: self.semaphore.clone(),
        }
    }
}

fn save_results<T: Serialize>(data: &T, output_path: &str) -> Result<(), ScraperError> {
    let json = serde_json::to_string_pretty(data)
        .map_err(|e| ScraperError::IoError(std::io::Error::new(std::io::ErrorKind::Other, e)))?;
    
    std::fs::write(output_path, json)
        .map_err(ScraperError::IoError)?;
    
    println!("Results saved to: {}", output_path);
    Ok(())
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    let cli = Cli::parse();
    
    match cli.command {
        Commands::Scrape { url, selector, output } => {
            let scraper = WebScraper::new(1);
            
            match scraper.scrape_url(&url, selector.as_deref()).await {
                Ok(data) => {
                    if let Some(output_path) = output {
                        save_results(&data, &output_path)?;
                    } else {
                        println!("{:#?}", data);
                    }
                }
                Err(e) => {
                    eprintln!("Error scraping URL: {}", e);
                }
            }
        }
        Commands::Batch { urls, selector, max_concurrent, output } => {
            let max_concurrent = max_concurrent.unwrap_or(5);
            let scraper = WebScraper::new(max_concurrent);
            
            match scraper.scrape_multiple(urls, selector.as_deref()).await {
                Ok(data) => {
                    if let Some(output_path) = output {
                        save_results(&data, &output_path)?;
                    } else {
                        println!("Scraped {} URLs:", data.len());
                        for item in data {
                            println!("- {}: {}", item.url, item.title.as_deref().unwrap_or("No title"));
                        }
                    }
                }
                Err(e) => {
                    eprintln!("Error in batch scraping: {}", e);
                }
            }
        }
        Commands::Crawl { base_url, max_depth, max_pages, selector, output } => {
            let max_depth = max_depth.unwrap_or(2);
            let max_pages = max_pages.unwrap_or(10);
            let scraper = WebScraper::new(3);
            
            match scraper.crawl_website(&base_url, max_depth, max_pages, selector.as_deref()).await {
                Ok(result) => {
                    if let Some(output_path) = output {
                        save_results(&result, &output_path)?;
                    } else {
                        println!("Crawl completed:");
                        println!("  Base URL: {}", result.base_url);
                        println!("  Pages scraped: {}", result.pages_scraped);
                        println!("  Total links found: {}", result.total_links_found);
                    }
                }
                Err(e) => {
                    eprintln!("Error crawling website: {}", e);
                }
            }
        }
        Commands::Extract { urls, title, links, images, text, output } => {
            let scraper = WebScraper::new(5);
            
            match scraper.extract_specific_data(urls, title, links, images, text).await {
                Ok(data) => {
                    if let Some(output_path) = output {
                        save_results(&data, &output_path)?;
                    } else {
                        for item in data {
                            println!("URL: {}", item.url);
                            if title && item.title.is_some() {
                                println!("  Title: {}", item.title.unwrap());
                            }
                            if links && !item.links.is_empty() {
                                println!("  Links: {}", item.links.len());
                            }
                            if images && !item.images.is_empty() {
                                println!("  Images: {}", item.images.len());
                            }
                            if text && item.text.is_some() {
                                let text_preview = &item.text.unwrap();
                                let preview = if text_preview.len() > 100 {
                                    &text_preview[..100]
                                } else {
                                    text_preview
                                };
                                println!("  Text preview: {}...", preview);
                            }
                            println!();
                        }
                    }
                }
                Err(e) => {
                    eprintln!("Error extracting data: {}", e);
                }
            }
        }
    }
    
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[tokio::test]
    async fn test_scraper_creation() {
        let scraper = WebScraper::new(1);
        assert_eq!(scraper.semaphore.available_permits(), 1);
    }

    #[test]
    fn test_scraped_data_serialization() {
        let data = ScrapedData {
            url: "https://example.com".to_string(),
            title: Some("Example".to_string()),
            text: Some("Example text".to_string()),
            links: vec!["https://example.com/page1".to_string()],
            images: vec!["https://example.com/image.jpg".to_string()],
            timestamp: chrono::Utc::now(),
        };
        
        let json = serde_json::to_string(&data).unwrap();
        assert!(json.contains("example.com"));
    }
}