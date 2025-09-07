import Foundation

struct User: Codable { let id: Int; let name: String }

func wordCount(_ s: String) -> [String: Int] {
    var m: [String: Int] = [:]
    s.lowercased().split{ !$0.isLetter }.forEach { m[String($0), default: 0] += 1 }
    return m
}

print("=== Swift demo ===")
let users = [User(id: 1, name: "Ada"), User(id: 2, name: "Alan")]
let data = try JSONEncoder().encode(users)
print(String(data: data, encoding: .utf8)!)
print(wordCount("Hello hello world"))

let here = URL(fileURLWithPath: FileManager.default.currentDirectoryPath)
for (i, url) in (try FileManager.default.contentsOfDirectory(at: here, includingPropertiesForKeys: nil)).enumerated() where i < 3 {
    print("-", url.lastPathComponent)
}