const std = @import("std");
const builtin = @import("builtin");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const kit_mod = b.createModule(.{
        .target = target,
        .optimize = optimize,
        .link_libc = true,
    });
    kit_mod.addIncludePath(b.path("src"));
    kit_mod.addCSourceFile(.{
        .file = b.path("src/kit.c"),
    });

    switch (builtin.target.os.tag) {
        .windows => {
            kit_mod.linkSystemLibrary("gdi32", .{});
            kit_mod.linkSystemLibrary("winmm", .{});
        },
        .macos => kit_mod.linkFramework("Cocoa", .{}),
        .linux => kit_mod.linkSystemLibrary("X11", .{}),
        else => @panic("unsupported os"),
    }

    const kit_lib = b.addLibrary(.{
        .name = "kit",
        .linkage = .static,
        .root_module = kit_mod,
    });

    const exe_mod = b.createModule(.{
        .target = target,
        .optimize = optimize,
        .link_libc = true,
    });
    exe_mod.addCSourceFile(.{
        .file = b.path("src/main.c"),
    });
    exe_mod.linkLibrary(kit_lib);

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
