const std = @import("std");

pub fn main() !void {
    var out = std.io.getStdOut().writer();
    try out.print("Hello from std.io.print: {s} {d}\n", .{ "num", 42 });
}

test "print formatting" {
    const s = try std.fmt.allocPrint(std.testing.allocator, "{x}", .{255});
    defer std.testing.allocator.free(s);
    try std.testing.expectEqualStrings("ff", s);
}
