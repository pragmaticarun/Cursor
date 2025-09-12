use async_trait::async_trait;
use clap::{Parser, Subcommand};
use hyper::service::{make_service_fn, service_fn};
use hyper::{Body, Method, Request, Response, Server, StatusCode};
use serde::{Deserialize, Serialize};
use std::collections::HashMap;
use std::convert::Infallible;
use std::net::SocketAddr;
use std::sync::Arc;
use tokio::sync::RwLock;
use tower::ServiceBuilder;
use tower_http::cors::CorsLayer;
use tower_http::fs::ServeDir;
use tower_http::trace::TraceLayer;
use tracing::{info, warn};
use uuid::Uuid;

#[derive(Parser)]
#[command(name = "web-server")]
#[command(about = "A complete HTTP server with middleware system")]
struct Cli {
    #[arg(short, long, default_value = "127.0.0.1")]
    host: String,
    #[arg(short, long, default_value = "3000")]
    port: u16,
    #[arg(short, long)]
    static_dir: Option<String>,
    #[command(subcommand)]
    command: Option<Commands>,
}

#[derive(Subcommand)]
enum Commands {
    /// Start the server
    Start,
    /// Show server configuration
    Config,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
struct User {
    id: Uuid,
    name: String,
    email: String,
    created_at: chrono::DateTime<chrono::Utc>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
struct CreateUserRequest {
    name: String,
    email: String,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
struct UpdateUserRequest {
    name: Option<String>,
    email: Option<String>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
struct ApiResponse<T> {
    success: bool,
    data: Option<T>,
    error: Option<String>,
    message: Option<String>,
}

impl<T> ApiResponse<T> {
    fn success(data: T) -> Self {
        Self {
            success: true,
            data: Some(data),
            error: None,
            message: None,
        }
    }
    
    fn error(message: String) -> Self {
        Self {
            success: false,
            data: None,
            error: Some(message),
            message: None,
        }
    }
}

#[derive(Debug, Clone, Serialize, Deserialize)]
struct ServerStats {
    total_requests: u64,
    active_connections: usize,
    uptime_seconds: u64,
    memory_usage: u64,
}

// Middleware trait
#[async_trait]
trait Middleware: Send + Sync {
    async fn handle(&self, request: Request<Body>) -> Result<Response<Body>, hyper::Error>;
}

// Authentication middleware
struct AuthMiddleware {
    api_key: String,
}

impl AuthMiddleware {
    fn new(api_key: String) -> Self {
        Self { api_key }
    }
}

#[async_trait]
impl Middleware for AuthMiddleware {
    async fn handle(&self, request: Request<Body>) -> Result<Response<Body>, hyper::Error> {
        let auth_header = request.headers().get("Authorization");
        
        if let Some(header) = auth_header {
            if let Ok(header_str) = header.to_str() {
                if header_str == format!("Bearer {}", self.api_key) {
                    return Ok(Response::new(Body::empty()));
                }
            }
        }
        
        let response = ApiResponse::<()>::error("Unauthorized".to_string());
        let json = serde_json::to_string(&response).unwrap();
        
        Ok(Response::builder()
            .status(StatusCode::UNAUTHORIZED)
            .header("Content-Type", "application/json")
            .body(Body::from(json))
            .unwrap())
    }
}

// Rate limiting middleware
struct RateLimitMiddleware {
    requests_per_minute: u32,
    requests: Arc<RwLock<HashMap<String, Vec<chrono::DateTime<chrono::Utc>>>>>,
}

impl RateLimitMiddleware {
    fn new(requests_per_minute: u32) -> Self {
        Self {
            requests_per_minute,
            requests: Arc::new(RwLock::new(HashMap::new())),
        }
    }
}

#[async_trait]
impl Middleware for RateLimitMiddleware {
    async fn handle(&self, request: Request<Body>) -> Result<Response<Body>, hyper::Error> {
        let client_ip = request.headers()
            .get("X-Forwarded-For")
            .and_then(|h| h.to_str().ok())
            .unwrap_or("unknown");
        
        let now = chrono::Utc::now();
        let minute_ago = now - chrono::Duration::minutes(1);
        
        let mut requests = self.requests.write().await;
        let client_requests = requests.entry(client_ip.to_string()).or_insert_with(Vec::new);
        
        // Remove old requests
        client_requests.retain(|&time| time > minute_ago);
        
        if client_requests.len() >= self.requests_per_minute as usize {
            let response = ApiResponse::<()>::error("Rate limit exceeded".to_string());
            let json = serde_json::to_string(&response).unwrap();
            
            return Ok(Response::builder()
                .status(StatusCode::TOO_MANY_REQUESTS)
                .header("Content-Type", "application/json")
                .body(Body::from(json))
                .unwrap());
        }
        
        client_requests.push(now);
        Ok(Response::new(Body::empty()))
    }
}

// Application state
struct AppState {
    users: Arc<RwLock<HashMap<Uuid, User>>>,
    stats: Arc<RwLock<ServerStats>>,
    start_time: chrono::DateTime<chrono::Utc>,
}

impl AppState {
    fn new() -> Self {
        Self {
            users: Arc::new(RwLock::new(HashMap::new())),
            stats: Arc::new(RwLock::new(ServerStats {
                total_requests: 0,
                active_connections: 0,
                uptime_seconds: 0,
                memory_usage: 0,
            })),
            start_time: chrono::Utc::now(),
        }
    }
    
    async fn get_stats(&self) -> ServerStats {
        let mut stats = self.stats.read().await.clone();
        stats.uptime_seconds = (chrono::Utc::now() - self.start_time).num_seconds() as u64;
        stats
    }
    
    async fn increment_requests(&self) {
        let mut stats = self.stats.write().await;
        stats.total_requests += 1;
    }
}

// Route handlers
async fn handle_users(request: Request<Body>, state: Arc<AppState>) -> Result<Response<Body>, hyper::Error> {
    state.increment_requests().await;
    
    match (request.method(), request.uri().path()) {
        (&Method::GET, "/api/users") => {
            let users = state.users.read().await;
            let user_list: Vec<User> = users.values().cloned().collect();
            let response = ApiResponse::success(user_list);
            let json = serde_json::to_string(&response).unwrap();
            
            Ok(Response::builder()
                .header("Content-Type", "application/json")
                .body(Body::from(json))
                .unwrap())
        }
        (&Method::POST, "/api/users") => {
            let body_bytes = hyper::body::to_bytes(request.into_body()).await?;
            let create_request: CreateUserRequest = match serde_json::from_slice(&body_bytes) {
                Ok(req) => req,
                Err(_) => {
                    let response = ApiResponse::<()>::error("Invalid JSON".to_string());
                    let json = serde_json::to_string(&response).unwrap();
                    return Ok(Response::builder()
                        .status(StatusCode::BAD_REQUEST)
                        .header("Content-Type", "application/json")
                        .body(Body::from(json))
                        .unwrap());
                }
            };
            
            let user = User {
                id: Uuid::new_v4(),
                name: create_request.name,
                email: create_request.email,
                created_at: chrono::Utc::now(),
            };
            
            let mut users = state.users.write().await;
            users.insert(user.id, user.clone());
            
            let response = ApiResponse::success(user);
            let json = serde_json::to_string(&response).unwrap();
            
            Ok(Response::builder()
                .status(StatusCode::CREATED)
                .header("Content-Type", "application/json")
                .body(Body::from(json))
                .unwrap())
        }
        (&Method::GET, path) if path.starts_with("/api/users/") => {
            let user_id_str = path.strip_prefix("/api/users/").unwrap();
            let user_id = match Uuid::parse_str(user_id_str) {
                Ok(id) => id,
                Err(_) => {
                    let response = ApiResponse::<()>::error("Invalid user ID".to_string());
                    let json = serde_json::to_string(&response).unwrap();
                    return Ok(Response::builder()
                        .status(StatusCode::BAD_REQUEST)
                        .header("Content-Type", "application/json")
                        .body(Body::from(json))
                        .unwrap());
                }
            };
            
            let users = state.users.read().await;
            match users.get(&user_id) {
                Some(user) => {
                    let response = ApiResponse::success(user.clone());
                    let json = serde_json::to_string(&response).unwrap();
                    Ok(Response::builder()
                        .header("Content-Type", "application/json")
                        .body(Body::from(json))
                        .unwrap())
                }
                None => {
                    let response = ApiResponse::<()>::error("User not found".to_string());
                    let json = serde_json::to_string(&response).unwrap();
                    Ok(Response::builder()
                        .status(StatusCode::NOT_FOUND)
                        .header("Content-Type", "application/json")
                        .body(Body::from(json))
                        .unwrap())
                }
            }
        }
        (&Method::PUT, path) if path.starts_with("/api/users/") => {
            let user_id_str = path.strip_prefix("/api/users/").unwrap();
            let user_id = match Uuid::parse_str(user_id_str) {
                Ok(id) => id,
                Err(_) => {
                    let response = ApiResponse::<()>::error("Invalid user ID".to_string());
                    let json = serde_json::to_string(&response).unwrap();
                    return Ok(Response::builder()
                        .status(StatusCode::BAD_REQUEST)
                        .header("Content-Type", "application/json")
                        .body(Body::from(json))
                        .unwrap());
                }
            };
            
            let body_bytes = hyper::body::to_bytes(request.into_body()).await?;
            let update_request: UpdateUserRequest = match serde_json::from_slice(&body_bytes) {
                Ok(req) => req,
                Err(_) => {
                    let response = ApiResponse::<()>::error("Invalid JSON".to_string());
                    let json = serde_json::to_string(&response).unwrap();
                    return Ok(Response::builder()
                        .status(StatusCode::BAD_REQUEST)
                        .header("Content-Type", "application/json")
                        .body(Body::from(json))
                        .unwrap());
                }
            };
            
            let mut users = state.users.write().await;
            match users.get_mut(&user_id) {
                Some(user) => {
                    if let Some(name) = update_request.name {
                        user.name = name;
                    }
                    if let Some(email) = update_request.email {
                        user.email = email;
                    }
                    
                    let response = ApiResponse::success(user.clone());
                    let json = serde_json::to_string(&response).unwrap();
                    Ok(Response::builder()
                        .header("Content-Type", "application/json")
                        .body(Body::from(json))
                        .unwrap())
                }
                None => {
                    let response = ApiResponse::<()>::error("User not found".to_string());
                    let json = serde_json::to_string(&response).unwrap();
                    Ok(Response::builder()
                        .status(StatusCode::NOT_FOUND)
                        .header("Content-Type", "application/json")
                        .body(Body::from(json))
                        .unwrap())
                }
            }
        }
        (&Method::DELETE, path) if path.starts_with("/api/users/") => {
            let user_id_str = path.strip_prefix("/api/users/").unwrap();
            let user_id = match Uuid::parse_str(user_id_str) {
                Ok(id) => id,
                Err(_) => {
                    let response = ApiResponse::<()>::error("Invalid user ID".to_string());
                    let json = serde_json::to_string(&response).unwrap();
                    return Ok(Response::builder()
                        .status(StatusCode::BAD_REQUEST)
                        .header("Content-Type", "application/json")
                        .body(Body::from(json))
                        .unwrap());
                }
            };
            
            let mut users = state.users.write().await;
            if users.remove(&user_id).is_some() {
                let response = ApiResponse::success(());
                let json = serde_json::to_string(&response).unwrap();
                Ok(Response::builder()
                    .status(StatusCode::NO_CONTENT)
                    .header("Content-Type", "application/json")
                    .body(Body::from(json))
                    .unwrap())
            } else {
                let response = ApiResponse::<()>::error("User not found".to_string());
                let json = serde_json::to_string(&response).unwrap();
                Ok(Response::builder()
                    .status(StatusCode::NOT_FOUND)
                    .header("Content-Type", "application/json")
                    .body(Body::from(json))
                    .unwrap())
            }
        }
        _ => {
            let response = ApiResponse::<()>::error("Not found".to_string());
            let json = serde_json::to_string(&response).unwrap();
            Ok(Response::builder()
                .status(StatusCode::NOT_FOUND)
                .header("Content-Type", "application/json")
                .body(Body::from(json))
                .unwrap())
        }
    }
}

async fn handle_stats(request: Request<Body>, state: Arc<AppState>) -> Result<Response<Body>, hyper::Error> {
    state.increment_requests().await;
    
    if request.method() == &Method::GET && request.uri().path() == "/api/stats" {
        let stats = state.get_stats().await;
        let response = ApiResponse::success(stats);
        let json = serde_json::to_string(&response).unwrap();
        
        Ok(Response::builder()
            .header("Content-Type", "application/json")
            .body(Body::from(json))
            .unwrap())
    } else {
        let response = ApiResponse::<()>::error("Not found".to_string());
        let json = serde_json::to_string(&response).unwrap();
        Ok(Response::builder()
            .status(StatusCode::NOT_FOUND)
            .header("Content-Type", "application/json")
            .body(Body::from(json))
            .unwrap())
    }
}

async fn handle_request(request: Request<Body>, state: Arc<AppState>) -> Result<Response<Body>, hyper::Error> {
    let path = request.uri().path();
    
    if path.starts_with("/api/users") {
        handle_users(request, state).await
    } else if path.starts_with("/api/stats") {
        handle_stats(request, state).await
    } else {
        // Serve static files or return 404
        let response = ApiResponse::<()>::error("Not found".to_string());
        let json = serde_json::to_string(&response).unwrap();
        Ok(Response::builder()
            .status(StatusCode::NOT_FOUND)
            .header("Content-Type", "application/json")
            .body(Body::from(json))
            .unwrap())
    }
}

async fn start_server(host: String, port: u16, static_dir: Option<String>) -> Result<(), Box<dyn std::error::Error>> {
    let addr = SocketAddr::from(([0, 0, 0, 0], port));
    let state = Arc::new(AppState::new());
    
    // Create service
    let make_svc = make_service_fn(move |_conn| {
        let state = state.clone();
        async move {
            Ok::<_, Infallible>(service_fn(move |req| {
                let state = state.clone();
                async move { handle_request(req, state).await }
            }))
        }
    });
    
    // Add middleware
    let service = ServiceBuilder::new()
        .layer(TraceLayer::new_for_http())
        .layer(CorsLayer::permissive());
    
    let server = Server::bind(&addr).serve(make_svc);
    
    info!("Server running on http://{}:{}", host, port);
    
    if let Err(e) = server.await {
        warn!("Server error: {}", e);
    }
    
    Ok(())
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    // Initialize tracing
    tracing_subscriber::fmt::init();
    
    let cli = Cli::parse();
    
    match cli.command {
        Some(Commands::Start) | None => {
            start_server(cli.host, cli.port, cli.static_dir).await?;
        }
        Some(Commands::Config) => {
            println!("Server Configuration:");
            println!("  Host: {}", cli.host);
            println!("  Port: {}", cli.port);
            if let Some(dir) = cli.static_dir {
                println!("  Static Directory: {}", dir);
            }
            println!("  API Endpoints:");
            println!("    GET    /api/users     - List all users");
            println!("    POST   /api/users     - Create a new user");
            println!("    GET    /api/users/:id - Get user by ID");
            println!("    PUT    /api/users/:id - Update user by ID");
            println!("    DELETE /api/users/:id - Delete user by ID");
            println!("    GET    /api/stats     - Get server statistics");
        }
    }
    
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;
    use hyper::Body;

    #[tokio::test]
    async fn test_user_creation() {
        let state = Arc::new(AppState::new());
        let user_data = CreateUserRequest {
            name: "Test User".to_string(),
            email: "test@example.com".to_string(),
        };
        
        let json = serde_json::to_string(&user_data).unwrap();
        let request = Request::builder()
            .method(Method::POST)
            .uri("/api/users")
            .header("Content-Type", "application/json")
            .body(Body::from(json))
            .unwrap();
        
        let response = handle_users(request, state).await.unwrap();
        assert_eq!(response.status(), StatusCode::CREATED);
    }

    #[tokio::test]
    async fn test_user_list() {
        let state = Arc::new(AppState::new());
        let request = Request::builder()
            .method(Method::GET)
            .uri("/api/users")
            .body(Body::empty())
            .unwrap();
        
        let response = handle_users(request, state).await.unwrap();
        assert_eq!(response.status(), StatusCode::OK);
    }

    #[tokio::test]
    async fn test_stats() {
        let state = Arc::new(AppState::new());
        let request = Request::builder()
            .method(Method::GET)
            .uri("/api/stats")
            .body(Body::empty())
            .unwrap();
        
        let response = handle_stats(request, state).await.unwrap();
        assert_eq!(response.status(), StatusCode::OK);
    }
}