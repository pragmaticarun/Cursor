use thiserror::Error;

#[derive(Debug, Error)]
pub enum AppError {
    #[error("invalid id: {0}")]
    InvalidId(u64),
}

fn validate_id(id: u64) -> Result<u64, AppError> {
    if id == 0 { Err(AppError::InvalidId(id)) } else { Ok(id) }
}

pub fn run() -> anyhow::Result<()> {
    println!("-- custom error types");
    match validate_id(0) {
        Ok(_) => println!("ok"),
        Err(e) => println!("err: {e}"),
    }
    Ok(())
}

