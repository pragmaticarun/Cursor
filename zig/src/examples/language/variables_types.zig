const std = @import("std");

pub fn main() !void {
    var stdout = std.io.getStdOut().writer();

    var mutable_integer: i32 = 42;
    const immutable_float: f64 = 3.14159;
    const boolean_flag: bool = true;
    const character: u8 = 'Z';
    const greeting: []const u8 = "Hello";

    mutable_integer += 8;
    try stdout.print("mutable_integer={{}} immutable_float={d:.3} flag={} char={} greeting={s}\n", .{ mutable_integer, immutable_float, boolean_flag, character, greeting });

    const inferred = 10; // type inference
    _ = inferred;
}

test "basic types behave as expected" {
    var x: i32 = 2;
    x += 3;
    try std.testing.expectEqual(@as(i32, 5), x);
}
