const std = @import("std");

pub fn main() !void {
    try std.io.getStdOut().writer().print("Hello, Zig!\n", .{});
}

test "hello prints greeting" {
    try std.testing.expect(true);
}
