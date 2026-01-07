# libopenmpt-godot

Play tracker music files in your Godot 4 game.

## What is Tracker Music?

Tracker music is a type of music file format that was super popular in old DOS games, Amiga games, and the demoscene. Instead of storing audio like MP3s do, tracker files store patterns of notes and samples - kind of like MIDI but with the instruments built-in. This makes them tiny in size but still sound great!

**Supported formats:** MOD, XM, S3M, IT, MPTM, and 40+ other formats

**Famous games that used tracker music:**
- Unreal (1998) - used .umx files (actually .it files)
- Deus Ex - used tracker music
- Many DOS games from the 90s

## Building

### Prerequisites
- Python 3.6+
- SCons 4.0+
- C++17 compatible compiler
- Git

### Build Steps

1. Initialize and update the godot-cpp submodule:
```bash
git submodule update --init --recursive
```

2. Build the extension:
```bash
# Debug build
scons target=template_debug

# Release build
scons target=template_release
```

The compiled library will be placed in `demo/bin/`.

### Platform-specific builds

```bash
# macOS
scons platform=macos

# Windows
scons platform=windows

# Linux
scons platform=linux
```

## Usage

The extension provides a complete API for playing tracker music files (MOD, XM, S3M, IT, and many more formats).

### Basic Example

```gdscript
# Create and load a module
var stream = AudioStreamOpenMPT.new()
stream.load_from_file("res://music/song.mod")

# Play using AudioStreamPlayer
var player = AudioStreamPlayer.new()
add_child(player)
player.stream = stream
player.play()
```

### Advanced Playback Control

```gdscript
# Get the playback object for real-time control
var playback = player.get_stream_playback() as AudioStreamPlaybackOpenMPT

# Control tempo and pitch
playback.set_tempo_factor(1.5)  # Play 50% faster
playback.set_pitch_factor(1.2)  # Pitch up

# Loop control
playback.set_repeat_count(-1)   # Loop forever
playback.set_repeat_count(2)    # Play 2 times

# Pattern navigation
playback.set_position_order_row(4, 0)  # Jump to order 4, row 0
print("Current pattern: ", playback.get_current_pattern())
print("Current row: ", playback.get_current_row())
```

### Module Information

```gdscript
# Get metadata
print("Title: ", stream.get_title())
print("Artist: ", stream.get_artist())
print("Message: ", stream.get_message())
print("Duration: ", stream.get_length(), " seconds")
print("BPM: ", stream.get_bpm())

# Get module structure info
print("Channels: ", stream.get_num_channels())
print("Patterns: ", stream.get_num_patterns())
print("Orders: ", stream.get_num_orders())
print("Instruments: ", stream.get_num_instruments())
print("Samples: ", stream.get_num_samples())

# List all instruments
var instruments = stream.get_instrument_names()
for i in instruments.size():
    print("Instrument ", i, ": ", instruments[i])

# Also available:
# stream.get_sample_names()
# stream.get_channel_names()
# stream.get_pattern_names()
# stream.get_order_names()
```

### Loading from Memory

```gdscript
# Load from PackedByteArray
var file = FileAccess.open("res://music/song.xm", FileAccess.READ)
var data = file.get_buffer(file.get_length())
file.close()

var stream = AudioStreamOpenMPT.new()
stream.load_from_data(data)
```

## API Reference

### AudioStreamOpenMPT

Main resource class for tracker modules. Extends `AudioStream`.

#### Properties

- `PackedByteArray data` - The module file data
- `int mix_rate` - Sample rate for playback (default: 48000)

#### Methods

**Loading:**
- `Error load_from_file(String path)` - Load module from file
- `Error load_from_data(PackedByteArray data)` - Load module from memory

**Metadata:**
- `String get_title()` - Get module title
- `String get_artist()` - Get artist/author name
- `String get_message()` - Get module message/comments
- `float get_length()` - Get duration in seconds
- `float get_bpm()` - Get current tempo

**Module Structure:**
- `int get_num_channels()` - Get number of channels
- `int get_num_orders()` - Get number of orders in the sequence
- `int get_num_patterns()` - Get number of patterns
- `int get_num_instruments()` - Get number of instruments
- `int get_num_samples()` - Get number of samples

**Names/Lists:**
- `PackedStringArray get_channel_names()` - Get all channel names
- `PackedStringArray get_order_names()` - Get all order names
- `PackedStringArray get_pattern_names()` - Get all pattern names
- `PackedStringArray get_instrument_names()` - Get all instrument names
- `PackedStringArray get_sample_names()` - Get all sample names

### AudioStreamPlaybackOpenMPT

Playback control class. Extends `AudioStreamPlayback`.

#### Methods

**Playback Control:**
- `void set_position(float position)` - Seek to position in seconds
- `float get_position()` - Get current playback position
- `void set_repeat_count(int count)` - Set loop count (-1 for infinite)
- `int get_repeat_count()` - Get current loop count

**Real-time Effects:**
- `void set_tempo_factor(float factor)` - Change playback speed (1.0 = normal)
- `float get_tempo_factor()` - Get current tempo factor
- `void set_pitch_factor(float factor)` - Change pitch (1.0 = normal)
- `float get_pitch_factor()` - Get current pitch factor

**Pattern Navigation:**
- `int get_current_order()` - Get current order index
- `int get_current_pattern()` - Get current pattern index
- `int get_current_row()` - Get current row in pattern
- `void set_position_order_row(int order, int row)` - Jump to specific order and row

## Testing

Build and run unit tests:

```bash
# Build tests
scons tests=yes target=template_debug

# Run tests
./tests/run_tests
```

The project uses [doctest](https://github.com/doctest/doctest) for C++ unit testing.

## Documentation

Full documentation is available at:
- **[GitHub Pages](https://your-username.github.io/libopenmpt-godot/)** - Automatically built from the `dev` branch
- **[Read the Docs](https://libopenmpt-godot.readthedocs.io/)** (if configured)

### Build Documentation Locally

**Option 1: Using Docker (Recommended)**

```bash
cd docs
./build.sh
```

Or manually with docker-compose:

```bash
cd docs
docker-compose up --build
```

**Option 2: Manual Build**

```bash
cd docs
pip install -r requirements.txt
make html
```

Open `docs/build/html/index.html` in your browser to view the documentation.

The documentation includes:
- Getting Started Guide
- Complete API Reference
- Usage Examples
- Building Instructions
- Testing Guide

## Supported Formats

libopenmpt supports a wide variety of tracker formats including:
- MOD (ProTracker, NoiseTracker, etc.)
- XM (FastTracker II)
- S3M (ScreamTracker 3)
- IT (Impulse Tracker)
- MPTM (OpenMPT)
- And many more legacy formats

For a complete list, see the [libopenmpt documentation](https://lib.openmpt.org/libopenmpt/).

## Project Structure

```
libopenmpt-godot/
├── src/                          # C++ source files
│   ├── register_types.cpp        # Extension registration
│   ├── register_types.h
│   ├── audio_stream_openmpt.cpp  # AudioStream implementation
│   ├── audio_stream_openmpt.h
│   ├── openmpt_player.cpp        # Optional Node wrapper
│   └── openmpt_player.h
├── tests/                        # Unit tests
│   ├── doctest/                  # doctest framework (submodule)
│   ├── test_main.cpp             # Test runner
│   └── test_audio_stream.cpp     # AudioStream tests
├── docs/                         # Sphinx documentation
│   ├── source/                   # Documentation source files
│   ├── requirements.txt          # Python dependencies for docs
│   └── Makefile                  # Documentation build system
├── demo/
│   └── bin/                      # Compiled libraries and .gdextension file
├── godot-cpp/                    # Godot C++ bindings (submodule)
├── libopenmpt/                   # libopenmpt library (submodule)
├── SConstruct                    # Build configuration
└── .readthedocs.yml              # Read the Docs configuration
```

## Contributing

Contributions are welcome! Please:

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Run tests: `scons tests=yes && ./tests/run_tests`
5. Submit a pull request

## License

This project integrates libopenmpt which is licensed under the BSD license. See the libopenmpt repository for details.
