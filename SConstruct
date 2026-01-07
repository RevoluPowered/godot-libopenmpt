#!/usr/bin/env python
import os
import sys
import subprocess

env = SConscript("godot-cpp/SConstruct")

# Add our source include paths
env.Append(CPPPATH=["src/", "libopenmpt/"])

# Determine libopenmpt build target
if env["platform"] == "macos":
    libopenmpt_config = "macos"
elif env["platform"] == "windows":
    libopenmpt_config = "mingw-w64"
else:
    libopenmpt_config = "gcc"

libopenmpt_target = "debug" if env["target"] == "template_debug" else "release"

# Build libopenmpt library if it doesn't exist
libopenmpt_lib_dir = "libopenmpt/bin"
libopenmpt_lib_name = "libopenmpt.a"  # MinGW uses .a files like Unix
libopenmpt_lib_path = os.path.join(libopenmpt_lib_dir, libopenmpt_lib_name)

def build_libopenmpt(target, source, env):
    """Build libopenmpt using its Makefile"""

    # Determine architecture
    arch = env.get("arch", "universal")

    # Build for each architecture if universal
    if arch == "universal" and env["platform"] == "macos":
        archs = ["arm64", "x86_64"]
        lib_files = []

        for single_arch in archs:
            print(f"Building libopenmpt for {single_arch}...")
            build_cmd = [
                "make",
                "-C", "libopenmpt",
                "clean",
            ]
            subprocess.run(build_cmd, capture_output=True)

            build_cmd = [
                "make",
                "-C", "libopenmpt",
                f"CONFIG={libopenmpt_config}",
                "STATIC_LIB=1",
                "SHARED_LIB=0",
                "EXAMPLES=0",
                "OPENMPT123=0",
                "TEST=0",
                "NO_ZLIB=1",
                "NO_MPG123=1",
                "NO_OGG=1",
                "NO_VORBIS=1",
                "NO_VORBISFILE=1",
                "NO_MINIZ=1",
                "NO_MINIMP3=1",
                "NO_STBVORBIS=1",
                "NO_PORTAUDIO=1",
                "NO_PORTAUDIOCPP=1",
                "NO_PULSEAUDIO=1",
                "NO_SDL2=1",
                "NO_FLAC=1",
                "NO_SNDFILE=1",
            ]

            # Add architecture-specific flags via environment
            env_vars = os.environ.copy()
            env_vars['CXXFLAGS'] = f"-arch {single_arch}"
            env_vars['CFLAGS'] = f"-arch {single_arch}"
            env_vars['LDFLAGS'] = f"-arch {single_arch}"

            build_cmd.append('STDCXX=c++20')

            if env["target"] == "template_debug":
                build_cmd.append("DEBUG=1")
            else:
                build_cmd.append("DEBUG=0")
                build_cmd.append("OPTIMIZE=speed")

            print(f"Command: {' '.join(build_cmd)}")
            result = subprocess.run(build_cmd, capture_output=False, env=env_vars)

            if result.returncode != 0:
                print(f"ERROR: Failed to build libopenmpt for {single_arch}")
                return result.returncode

            # Copy the built library
            lib_file = f"libopenmpt/bin/libopenmpt_{single_arch}.a"
            subprocess.run(["cp", "libopenmpt/bin/libopenmpt.a", lib_file])
            lib_files.append(lib_file)

        # Create universal binary with lipo
        print("Creating universal binary...")
        lipo_cmd = ["lipo", "-create"] + lib_files + ["-output", str(target[0])]
        result = subprocess.run(lipo_cmd)

        if result.returncode != 0:
            print("ERROR: Failed to create universal binary")
            return result.returncode

        return 0
    else:
        # Single architecture build
        build_cmd = [
            "make",
            "-C", "libopenmpt",
            f"CONFIG={libopenmpt_config}",
            "STATIC_LIB=1",
            "SHARED_LIB=0",
            "EXAMPLES=0",
            "OPENMPT123=0",
            "TEST=0",
            "NO_ZLIB=1",
            "NO_MPG123=1",
            "NO_OGG=1",
            "NO_VORBIS=1",
            "NO_VORBISFILE=1",
            "NO_MINIZ=1",
            "NO_MINIMP3=1",
            "NO_STBVORBIS=1",
            "NO_PORTAUDIO=1",
            "NO_PORTAUDIOCPP=1",
            "NO_PULSEAUDIO=1",
            "NO_SDL2=1",
            "NO_FLAC=1",
            "NO_SNDFILE=1",
        ]

        # Add Windows architecture specification
        if env["platform"] == "windows":
            if env.get("arch") == "x86_64":
                build_cmd.append("WINDOWS_ARCH=amd64")
            elif env.get("arch") == "x86_32":
                build_cmd.append("WINDOWS_ARCH=x86")

        if env["target"] == "template_debug":
            build_cmd.append("DEBUG=1")
        else:
            build_cmd.append("DEBUG=0")
            build_cmd.append("OPTIMIZE=speed")

        # Set up environment variables for the build
        env_vars = os.environ.copy()
        if env["platform"] == "windows":
            env_vars['AR'] = 'ar'

        print(f"Building libopenmpt: {' '.join(build_cmd)}")
        result = subprocess.run(build_cmd, capture_output=False, env=env_vars)

        if result.returncode != 0:
            print("ERROR: Failed to build libopenmpt")
            return result.returncode

        # Copy to target location if different
        if str(target[0]) != "libopenmpt/bin/libopenmpt.a":
            subprocess.run(["cp", "libopenmpt/bin/libopenmpt.a", str(target[0])])

        return 0

# Check if libopenmpt library exists, if not build it
if not os.path.exists(libopenmpt_lib_path):
    print(f"libopenmpt library not found at {libopenmpt_lib_path}, building...")
    # Create a dummy builder to trigger libopenmpt build
    libopenmpt_builder = env.Command(
        libopenmpt_lib_path,
        "libopenmpt/Makefile",
        build_libopenmpt
    )
    env.Depends("src/audio_stream_openmpt.cpp", libopenmpt_builder)
else:
    print(f"Using existing libopenmpt library: {libopenmpt_lib_path}")

# Link against libopenmpt static library
env.Append(LIBPATH=[libopenmpt_lib_dir])
env.Append(LIBS=["openmpt"])

# Add C++ standard library for libopenmpt
if env["platform"] == "macos":
    env.Append(LIBS=["c++"])
elif env["platform"] == "linux":
    env.Append(LIBS=["stdc++"])
elif env["platform"] == "windows":
    env.Append(LIBS=["stdc++"])

# Add our GDExtension source files
sources = Glob("src/*.cpp")

# Check if tests should be built
build_tests = ARGUMENTS.get("tests", "no") == "yes"

# Prepare source list
lib_sources = list(sources)
if build_tests:
    # Enable tests in the build
    env.Append(CPPDEFINES=["TESTS_ENABLED"])
    env.Append(CPPPATH=["tests/doctest"])

    # Add test interface sources
    lib_sources.extend(["tests/test_interface.cpp", "tests/test_audio_stream.cpp"])

# Create the library
if env["platform"] == "macos":
    library = env.SharedLibrary(
        "demo/bin/libopenmpt.{}.{}.framework/libopenmpt.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=lib_sources,
    )
else:
    library = env.SharedLibrary(
        "demo/bin/libopenmpt{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=lib_sources,
    )

Default(library)
