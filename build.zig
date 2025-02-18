//! build.sh ported to Zig
const std = @import("std");
const builtin = @import("builtin");
const Build = std.Build;
const Step = Build.Step;

// Add steps to build and run kernel
pub fn build(b: *Build) !void {
    const kernel = try buildKernel(b);
    try runKernel(b, kernel);
}

// Builds the kernel and adds the install step for zig-out
fn buildKernel(b: *Build) !*Step.Compile {
    const target = b.resolveTargetQuery(.{
        .cpu_arch = .x86,
        .os_tag = .freestanding,
        .abi = .eabi,
    });
    const optimize = b.standardOptimizeOption(.{});

    const glu = b.addExecutable(.{
        .name = "glu.elf",
        .target = target,
        .optimize = optimize,
    });
    glu.addAssemblyFile(b.path("src/x86/boot.s"));
    glu.addCSourceFiles(.{
        .root = b.path("src"),
        .flags = &.{"-fno-sanitize=undefined"},
        .files = &.{
            "glu/io.c",
            "glu/mem.c",
            "glu/teletype.c",
            "x86/VGA.c",
            "x86/GDT.c",
            "kernel.c",
        },
    });
    glu.setLinkerScript(b.path("scripts/linker.zig.ld"));

    b.installArtifact(glu);
    return glu;
}

// Runs the kernel with qemu and attaches a debugger to it
fn runKernel(b: *std.Build, kernel: *Step.Compile) !void {
    var run_qemu = b.addSystemCommand(&.{ "qemu-system-i386", "-s", "-S", "-daemonize", "-kernel" });
    run_qemu.addArtifactArg(kernel);
    if (builtin.target.isDarwin()) {
        // For some reason, this environment variable has to be set on Mac for this to work
        run_qemu.setEnvironmentVariable("OBJC_DISABLE_INITIALIZE_FORK_SAFETY", "YES");
    }

    const run_gdb = runDebugger(b, kernel);
    run_gdb.step.dependOn(&run_qemu.step);

    var run_step = b.step("run", "Run and debug the kernel with QEMU");
    run_step.dependOn(&run_gdb.step);
}

// Generates the command to run the debugger and connect to the remote gdb server for qemu
fn runDebugger(b: *std.Build, kernel: *Step.Compile) *Step.Run {
    const debuggers: []const []const u8 = &.{ "gdb", "lldb" };
    const arguments: []const []const []const u8 = &.{
        &.{ "-ex", "target remote localhost:1234" },
        &.{ "--one-line", "gdb-remote localhost:1234" },
    };

    var run_gdb = for (debuggers, arguments) |dbg, args| {
        var out_code: u8 = 0;
        if (b.runAllowFail(&.{ dbg, "--version" }, &out_code, .Ignore)) |output| {
            b.allocator.free(output);
            if (out_code != 0) continue;
        } else |_| {
            continue;
        }

        var run_gdb = b.addSystemCommand(&.{dbg});
        run_gdb.addArgs(args);
        break run_gdb;
    } else {
        inline for (debuggers) |dbg| {
            std.debug.print("searched for {s}\n", .{dbg});
        }
        @panic("no debugger found on system!");
    };

    // Add the file "kernel" that we are actually debugging
    run_gdb.addFileArg(kernel.getEmittedBin());
    return run_gdb;
}
