const std = @import("std");

fn fib(n: u32) u64 {
    if (n <= 1) return n;
    var a: u64 = 0;
    var b: u64 = 1;
    var i: u32 = 0;
    while (i < n) : (i += 1) {
        const next = a + b;
        a = b;
        b = next;
    }
    return a;
}

pub fn main() !void {
    var stdout = std.io.getStdOut().writer();
    for ([_]u8{ 1, 2, 3, 4, 5 }, 0..) |v, idx| {
        try stdout.print("idx={} v={} fib(v)={}\n", .{ idx, v, fib(v) });
    }
}

test "fib small values" {
    try std.testing.expectEqual(@as(u64, 0), fib(0));
    try std.testing.expectEqual(@as(u64, 1), fib(1));
    try std.testing.expectEqual(@as(u64, 1), fib(2));
    try std.testing.expectEqual(@as(u64, 2), fib(3));
    try std.testing.expectEqual(@as(u64, 3), fib(4));
}
