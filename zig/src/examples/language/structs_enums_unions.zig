const std = @import("std");

const Color = enum { red, green, blue };

const Point = struct {
    x: f64,
    y: f64,
};

const Value = union(enum) {
    int: i64,
    float: f64,
    text: []const u8,
};

pub fn main() !void {
    var out = std.io.getStdOut().writer();
    const p = Point{ .x = 1.0, .y = 2.0 };
    const c = Color.blue;
    const v = Value{ .text = "zig" };
    try out.print("p=({d:.1},{d:.1}) c={any} v={any}\n", .{ p.x, p.y, c, v });
}

test "union usage" {
    var v = Value{ .int = 12 };
    try std.testing.expectEqual(@as(i64, 12), v.int);
    v = Value{ .float = 3.5 };
    try std.testing.expectEqual(@as(f64, 3.5), v.float);
}
