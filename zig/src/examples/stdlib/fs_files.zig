const std = @import("std");

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    var cwd = std.fs.cwd();
    try cwd.makePath("zig-out");
    var file = try cwd.createFile("zig-out/example.txt", .{ .read = true, .truncate = true, .mode = 0o644 });
    defer file.close();
    try file.writer().print("{s}\n", .{"hello fs"});
    try file.seekTo(0);
    const content = try file.readToEndAlloc(allocator, 1 << 16);
    defer allocator.free(content);
    try std.io.getStdOut().writer().print("read back: {s}", .{content});
}

test "fs create and read" {
    var tmp = std.testing.tmpDir(.{});
    defer tmp.cleanup();
    var f = try tmp.dir.createFile("a.txt", .{ .read = true });
    defer f.close();
    try f.writeAll("abc");
    try f.seekTo(0);
    var buf: [3]u8 = undefined;
    _ = try f.read(&buf);
    try std.testing.expectEqualStrings("abc", &buf);
}
