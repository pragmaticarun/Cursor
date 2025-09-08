const std = @import("std");

pub fn main() !void {
    var out = std.io.getStdOut().writer();
    try out.print("sleeping...\n", .{});
    std.time.sleep(100 * std.time.ns_per_ms);
    try out.print("awake\n", .{});
}

test "sleep does not crash" {
    std.time.sleep(10 * std.time.ns_per_ms);
    try std.testing.expect(true);
}
