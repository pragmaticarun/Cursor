const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const lang_examples = [_]struct { name: []const u8, src: []const u8 }{
        .{ .name = "lang-hello", .src = "src/examples/language/hello.zig" },
        .{ .name = "lang-variables-types", .src = "src/examples/language/variables_types.zig" },
        .{ .name = "lang-control-flow", .src = "src/examples/language/control_flow.zig" },
        .{ .name = "lang-errors", .src = "src/examples/language/errors.zig" },
        .{ .name = "lang-defer-errdefer", .src = "src/examples/language/defer_errdefer.zig" },
        .{ .name = "lang-structs-enums-unions", .src = "src/examples/language/structs_enums_unions.zig" },
        .{ .name = "lang-pointers-arrays-slices", .src = "src/examples/language/pointers_arrays_slices.zig" },
        .{ .name = "lang-comptime-generics", .src = "src/examples/language/comptime_generics.zig" },
        .{ .name = "lang-switch", .src = "src/examples/language/switch.zig" },
    };

    const std_examples = [_]struct { name: []const u8, src: []const u8 }{
        .{ .name = "std-io-print", .src = "src/examples/stdlib/io_print.zig" },
        .{ .name = "std-fs-files", .src = "src/examples/stdlib/fs_files.zig" },
        .{ .name = "std-time-sleep", .src = "src/examples/stdlib/time_sleep.zig" },
        .{ .name = "std-rand-numbers", .src = "src/examples/stdlib/rand_numbers.zig" },
        .{ .name = "std-json-encode-decode", .src = "src/examples/stdlib/json_encode_decode.zig" },
        .{ .name = "std-mem-slices", .src = "src/examples/stdlib/mem_slices.zig" },
        .{ .name = "std-heap-allocator", .src = "src/examples/stdlib/heap_allocator.zig" },
    };

    const run_all = b.step("run-all", "Run all example programs");
    const fmt_step = b.step("fmt", "Format all Zig sources");
    const fmt_cmd = b.addSystemCommand(&[_][]const u8{ "zig", "fmt", "." });
    fmt_step.dependOn(&fmt_cmd.step);

    // Language examples
    inline for (lang_examples) |ex| {
        const exe = b.addExecutable(.{
            .name = ex.name,
            .root_source_file = .{ .path = ex.src },
            .target = target,
            .optimize = optimize,
        });
        b.installArtifact(exe);
        const run_cmd = b.addRunArtifact(exe);
        if (b.args) |args| run_cmd.addArgs(args);
        const run_step = b.step(b.fmt("run-{s}", .{ex.name}), b.fmt("Run {s}", .{ex.name}));
        run_step.dependOn(&run_cmd.step);
        run_all.dependOn(&run_cmd.step);

        const t = b.addTest(.{
            .root_source_file = .{ .path = ex.src },
            .target = target,
            .optimize = optimize,
        });
        const run_tests = b.addRunArtifact(t);
        const test_step = b.step(b.fmt("test-{s}", .{ex.name}), b.fmt("Test {s}", .{ex.name}));
        test_step.dependOn(&run_tests.step);
    }

    // Stdlib examples
    inline for (std_examples) |ex| {
        const exe = b.addExecutable(.{
            .name = ex.name,
            .root_source_file = .{ .path = ex.src },
            .target = target,
            .optimize = optimize,
        });
        b.installArtifact(exe);
        const run_cmd = b.addRunArtifact(exe);
        if (b.args) |args| run_cmd.addArgs(args);
        const run_step = b.step(b.fmt("run-{s}", .{ex.name}), b.fmt("Run {s}", .{ex.name}));
        run_step.dependOn(&run_cmd.step);
        run_all.dependOn(&run_cmd.step);

        const t = b.addTest(.{
            .root_source_file = .{ .path = ex.src },
            .target = target,
            .optimize = optimize,
        });
        const run_tests = b.addRunArtifact(t);
        const test_step = b.step(b.fmt("test-{s}", .{ex.name}), b.fmt("Test {s}", .{ex.name}));
        test_step.dependOn(&run_tests.step);
    }

    // Aggregate test step
    const all_tests = b.step("test", "Run tests for all examples");
    inline for (lang_examples) |ex| {
        const t = b.addTest(.{ .root_source_file = .{ .path = ex.src }, .target = target, .optimize = optimize });
        const run_tests = b.addRunArtifact(t);
        all_tests.dependOn(&run_tests.step);
    }
    inline for (std_examples) |ex| {
        const t = b.addTest(.{ .root_source_file = .{ .path = ex.src }, .target = target, .optimize = optimize });
        const run_tests = b.addRunArtifact(t);
        all_tests.dependOn(&run_tests.step);
    }
}
