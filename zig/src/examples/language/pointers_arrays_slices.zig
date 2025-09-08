const std = @import("std");

fn sumSlice(slice: []const i32) i32 {
    var total: i32 = 0;
    for (slice) |v| total += v;
    return total;
}

pub fn main() !void {
    var out = std.io.getStdOut().writer();
    var arr = [_]i32{ 1, 2, 3, 4 };
    const s: []i32 = arr[1..];
    s[0] = 10;
    try out.print("sum={} arr0={}\n", .{ sumSlice(arr[0..]), arr[0] });
}

test "sumSlice" {
    const a = [_]i32{ 1, 2, 3 };
    try std.testing.expectEqual(@as(i32, 6), sumSlice(a[0..]));
}
