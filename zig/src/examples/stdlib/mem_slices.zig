const std = @import("std");

pub fn main() !void {
    var out = std.io.getStdOut().writer();
    const haystack = "one,two,three";
    var it = std.mem.splitScalar(u8, haystack, ',');
    while (it.next()) |part| {
        try out.print("[{s}]\n", .{part});
    }
}

test "splitScalar" {
    const s = "a,b";
    var it = std.mem.splitScalar(u8, s, ',');
    try std.testing.expectEqualStrings("a", it.next().?);
    try std.testing.expectEqualStrings("b", it.next().?);
    try std.testing.expect(it.next() == null);
}
