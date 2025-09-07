#include <cstdio>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <vector>

class File {
public:
    explicit File(const char* path, const char* mode) : h(std::fopen(path, mode)) {
        if (!h) throw std::runtime_error("open failed");
    }
    File(const File&) = delete;
    File& operator=(const File&) = delete;
    File(File&& o) noexcept : h(o.h) { o.h = nullptr; }
    File& operator=(File&& o) noexcept { if (this!=&o){ close(); h=o.h; o.h=nullptr;} return *this; }
    ~File(){ close(); }
    std::FILE* get() const { return h; }
private:
    std::FILE* h{};
    void close(){ if(h){ std::fclose(h); h=nullptr; } }
};

int main(){
    std::string path = "tmp.txt";
    { File f(path.c_str(), "w"); std::fputs("hello", f.get()); }
    for (auto& p : std::filesystem::directory_iterator(".")) {
        (void)p;
    }
}