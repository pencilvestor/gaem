const std = @import("std");
const builtin = @import("builtin");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const engine_mod = b.addModule("engine", .{
        .root_source_file = b.path("src/engine.zig"),
        .target = target,
        .optimize = optimize,
        .link_libc = true,
    });
    engine_mod.addIncludePath(b.path("src"));
    engine_mod.addCSourceFile(.{
        .file = b.path("src/kit.c"),
    });

    switch (builtin.target.os.tag) {
        .windows => {
            engine_mod.linkSystemLibrary("gdi32", .{});
            engine_mod.linkSystemLibrary("winmm", .{});
        },
        .macos => engine_mod.linkFramework("Cocoa", .{}),
        .linux => engine_mod.linkSystemLibrary("X11", .{}),
        else => @panic("unsupported os"),
    }

    const exe_mod = b.createModule(.{
        .root_source_file = b.path("src/main.zig"),
        .target = target,
        .optimize = optimize,
    });
    exe_mod.addImport("engine", engine_mod);

    const exe = b.addExecutable(.{
        .name = "gaem",
        .root_module = exe_mod,
    });

    b.installArtifact(exe);

    const run_cmd = b.addRunArtifact(exe);

    run_cmd.step.dependOn(b.getInstallStep());

    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);
}
