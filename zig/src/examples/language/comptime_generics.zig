const std = @import("std");

fn add(T: type, a: T, b: T) T {
    return a + b;
}

fn arrayOf(comptime T: type, comptime N: usize, value: T) [N]T {
    var arr: [N]T = undefined;
    for (&arr) |*item| item.* = value;
    return arr;
}

pub fn main() !void {
    var out = std.io.getStdOut().writer();
    const x = add(i32, 2, 3);
    const y = add(f64, 1.5, 2.25);
    const arr = arrayOf(u8, 4, 7);
    try out.print("x={} y={d:.2} arr={any}\n", .{ x, y, arr });
}

test "comptime generics" {
    try std.testing.expectEqual(@as(i32, 5), add(i32, 2, 3));
    const arr = arrayOf(u8, 3, 9);
    try std.testing.expectEqual(@as(u8, 9), arr[0]);
}
