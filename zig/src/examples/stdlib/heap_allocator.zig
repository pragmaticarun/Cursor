const std = @import("std");

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    var list = std.ArrayList(u32).init(allocator);
    defer list.deinit();
    try list.appendSlice(&[_]u32{ 1, 2, 3 });
    try std.io.getStdOut().writer().print("list={any}\n", .{list.items});
}

test "allocator works" {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();
    var list = std.ArrayList(u8).init(allocator);
    defer list.deinit();
    try list.append(7);
    try std.testing.expectEqual(@as(usize, 1), list.items.len);
}
