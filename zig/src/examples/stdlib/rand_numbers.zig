const std = @import("std");

pub fn main() !void {
    var out = std.io.getStdOut().writer();
    var prng = std.rand.DefaultPrng.init(12345);
    var r = prng.random();
    try out.print("u32={} f64={d:.3}\n", .{ r.int(u32), r.float(f64) });
}

test "rand deterministic" {
    var prng = std.rand.DefaultPrng.init(1);
    var r = prng.random();
    const a = r.int(u32);
    var prng2 = std.rand.DefaultPrng.init(1);
    try std.testing.expectEqual(a, prng2.random().int(u32));
}
