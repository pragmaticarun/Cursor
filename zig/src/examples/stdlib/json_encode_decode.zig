const std = @import("std");

const Person = struct {
    name: []const u8,
    age: u8,
};

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    const p = Person{ .name = "Zig", .age = 10 };
    var buf = std.ArrayList(u8).init(allocator);
    defer buf.deinit();
    try std.json.stringify(p, .{}, buf.writer());
    const encoded = try buf.toOwnedSlice();
    defer allocator.free(encoded);
    try std.io.getStdOut().writer().print("json={s}\n", .{encoded});

    var parsed = try std.json.parseFromSlice(Person, allocator, encoded, .{});
    defer parsed.deinit();
    try std.io.getStdOut().writer().print("name={s} age={}\n", .{ parsed.value.name, parsed.value.age });
}

test "json roundtrip" {
    var a = std.heap.FixedBufferAllocator.init(&[_]u8{0} ** 256);
    const allocator = a.allocator();
    const p = Person{ .name = "A", .age = 1 };
    var buf = std.ArrayList(u8).init(allocator);
    defer buf.deinit();
    try std.json.stringify(p, .{}, buf.writer());
    const s = buf.items;
    var parsed = try std.json.parseFromSlice(Person, allocator, s, .{});
    defer parsed.deinit();
    try std.testing.expectEqual(@as(u8, 1), parsed.value.age);
}
