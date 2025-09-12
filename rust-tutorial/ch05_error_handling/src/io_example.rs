use std::{fs, path::PathBuf};

pub fn run() -> anyhow::Result<()> {
    println!("-- io + ?");
    let mut path = PathBuf::from(env!("CARGO_MANIFEST_DIR"));
    path.push("README.md");
    let _ = fs::write(&path, b"error handling demo\n");
    let content = fs::read_to_string(&path)?;
    println!("read {} bytes", content.len());
    Ok(())
}

