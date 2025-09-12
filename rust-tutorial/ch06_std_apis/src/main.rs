mod threads_channels;
mod fs_io;

fn main() {
    println!("== Chapter 06: Standard APIs ==");
    fs_io::run();
    threads_channels::run();
}

