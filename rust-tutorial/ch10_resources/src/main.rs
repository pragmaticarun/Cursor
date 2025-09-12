use serde::Deserialize;

#[derive(Debug, Deserialize)]
struct HttpBin {
    url: String,
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    println!("== Chapter 10: Community & Resources (HTTP demo) ==");
    let resp = reqwest::get("https://httpbin.org/get").await?;
    let parsed: HttpBin = resp.json().await?;
    println!("fetched: {}", parsed.url);
    println!("See The Book, Rust by Example, crates.io, docs.rs, and forums");
    Ok(())
}

