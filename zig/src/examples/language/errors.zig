const std = @import("std");

const MyError = error{ Fail, NotFound };

fn mightFail(succeed: bool) MyError!u32 {
    if (!succeed) return MyError.Fail;
    return 123;
}

pub fn main() !void {
    var stdout = std.io.getStdOut().writer();
    const result = mightFail(false) catch |err| blk: {
        try stdout.print("caught error: {s}\n", .{@errorName(err)});
        break :blk 0;
    };
    try stdout.print("result={}\n", .{result});
}

test "mightFail" {
    try std.testing.expectError(MyError.Fail, mightFail(false));
    try std.testing.expectEqual(@as(u32, 123), try mightFail(true));
}
