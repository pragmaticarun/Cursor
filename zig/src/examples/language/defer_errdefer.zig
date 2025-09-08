const std = @import("std");

fn doThing(fail: bool) !void {
    var stdout = std.io.getStdOut().writer();
    try stdout.print("start\n", .{});
    defer stdout.print("defer always runs\n", .{}) catch {};
    errdefer stdout.print("errdefer only on error\n", .{}) catch {};
    if (fail) return error.Sad;
    try stdout.print("success path\n", .{});
}

pub fn main() !void {
    _ = doThing(false) catch {};
    _ = doThing(true) catch {};
}

test "errdefer triggers only on error" {
    try doThing(false);
    try std.testing.expectError(error.Sad, doThing(true));
}
