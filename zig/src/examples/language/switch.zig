const std = @import("std");

fn classify(n: i32) []const u8 {
    return switch (n) {
        -100...-1 => "negative",
        0 => "zero",
        1...100 => "positive",
        else => "big",
    };
}

pub fn main() !void {
    var out = std.io.getStdOut().writer();
    try out.print("{s} {s} {s}\n", .{ classify(-5), classify(0), classify(7) });
}

test "classify" {
    try std.testing.expectEqualStrings("negative", classify(-1));
    try std.testing.expectEqualStrings("zero", classify(0));
    try std.testing.expectEqualStrings("positive", classify(5));
}
