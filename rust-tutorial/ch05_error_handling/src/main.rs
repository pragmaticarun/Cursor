mod io_example;
mod custom_error;

fn main() -> anyhow::Result<()> {
    println!("== Chapter 05: Error Handling ==");
    io_example::run()?;
    custom_error::run()?;
    Ok(())
}

