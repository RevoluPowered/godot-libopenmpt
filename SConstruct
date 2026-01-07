#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

# Add libopenmpt include paths
env.Append(CPPPATH=[
    "src/",
    "libopenmpt/",
    "libopenmpt/libopenmpt/",
    "libopenmpt/common/",
    "libopenmpt/soundlib/",
])

# Add libopenmpt defines
env.Append(CPPDEFINES=[
    "LIBOPENMPT_BUILD",
    "MPT_WITH_ZLIB",
    "MPT_WITH_MPG123",
])

# Collect libopenmpt source files
libopenmpt_sources = [
    "libopenmpt/libopenmpt/libopenmpt_c.cpp",
    "libopenmpt/libopenmpt/libopenmpt_cxx.cpp",
    "libopenmpt/libopenmpt/libopenmpt_impl.cpp",
    "libopenmpt/libopenmpt/libopenmpt_ext_impl.cpp",
]

# Collect soundlib sources (required for libopenmpt)
soundlib_sources = Glob("libopenmpt/soundlib/*.cpp")
common_sources = Glob("libopenmpt/common/*.cpp")

# Add our GDExtension source files
sources = Glob("src/*.cpp")

# Combine all sources
all_sources = sources + libopenmpt_sources + soundlib_sources + common_sources

# Check if tests should be built
build_tests = ARGUMENTS.get("tests", "no") == "yes"

if build_tests:
    # Build unit tests
    test_env = env.Clone()
    test_env.Append(CPPPATH=["tests/doctest"])

    test_sources = Glob("tests/*.cpp")
    test_program = test_env.Program(
        "tests/run_tests",
        source=test_sources + all_sources
    )

    Default(test_program)
else:
    # Create the library
    if env["platform"] == "macos":
        library = env.SharedLibrary(
            "demo/bin/libopenmpt.{}.{}.framework/libopenmpt.{}.{}".format(
                env["platform"], env["target"], env["platform"], env["target"]
            ),
            source=all_sources,
        )
    else:
        library = env.SharedLibrary(
            "demo/bin/libopenmpt{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
            source=all_sources,
        )

    Default(library)
