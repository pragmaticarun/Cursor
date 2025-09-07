import Foundation

struct Item: Codable { let id: Int, let name: String }

let squares = (1...5).map { $0 * $0 }.filter { $0 % 2 == 1 }
print(squares)

let json = try! JSONEncoder().encode(Item(id: 1, name: "Swift"))
print(String(data: json, encoding: .utf8)!)