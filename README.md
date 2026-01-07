# libopenmpt-godot

[![Build Status](https://github.com/RevoluPowered/godot-libopenmpt/actions/workflows/build.yml/badge.svg)](https://github.com/RevoluPowered/godot-libopenmpt/actions/workflows/build.yml)
[![Documentation](https://github.com/RevoluPowered/godot-libopenmpt/actions/workflows/docs.yml/badge.svg)](https://github.com/RevoluPowered/godot-libopenmpt/actions/workflows/docs.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

**[📖 Documentation](https://revolupowered.github.io/godot-libopenmpt/)** | **[🎵 Download](https://github.com/RevoluPowered/godot-libopenmpt/releases)**

Play tracker music files in your Godot 4 game.

## What is Tracker Music?

Tracker music is a type of music file format that was super popular in old DOS games, Amiga games, and the demoscene. Instead of storing audio like MP3s do, tracker files store patterns of notes and samples - kind of like MIDI but with the instruments built-in. This makes them tiny in size but still sound great!

**Supported formats:** MOD, XM, S3M, IT, MPTM, and 40+ other formats

**Famous games that used tracker music:**
- Unreal (1998) - used .umx files (actually .it files)
- Deus Ex - used tracker music
- Many DOS games from the 90s

## How to Get It

### Download (Easiest!)

1. Go to [Releases](https://github.com/RevoluPowered/libopenmpt-godot/releases) and download the latest version
2. Unzip it
3. Copy the `addons/libopenmpt/` folder into your Godot project
4. Restart Godot

Done! Skip to the "How to Use" section below.

### Build It Yourself

#### Building Manually

**What you need:**
- Python 3
- A C++ compiler (Visual Studio on Windows, Xcode on Mac, gcc on Linux)
- Git

**Steps:**

```bash
# 1. Download the code
git clone --recursive https://github.com/RevoluPowered/libopenmpt-godot.git
cd libopenmpt-godot

# 2. Install the build tool
pip install scons

# 3. Build it
scons target=template_release
```

Files will appear in `demo/bin/`.

#### Copy Plugin to Your Game

After building, copy it to your project:

```bash
mkdir -p MyGame/addons/libopenmpt/
cp -r demo/bin/* MyGame/addons/libopenmpt/
```

(Replace `MyGame` with your actual project folder name)

## How to Use

### Simple Example - Just Play Music!

```gdscript
# Load a tracker file
var stream = AudioStreamOpenMPT.new()
stream.load_from_file("res://music/cool_song.mod")

# Play it like any other audio
var player = AudioStreamPlayer.new()
add_child(player)
player.stream = stream
player.play()
```

That's it! Your tracker music is now playing.

### Cool Stuff You Can Do

**Speed up or slow down the music:**
```gdscript
var playback = player.get_stream_playback()
playback.set_tempo_factor(1.5)  # 50% faster
playback.set_tempo_factor(0.5)  # Half speed
```

**Change the pitch:**
```gdscript
playback.set_pitch_factor(1.5)  # Higher pitch
playback.set_pitch_factor(0.8)  # Lower pitch
```

**Loop the music:**
```gdscript
playback.set_repeat_count(-1)  # Loop forever
playback.set_repeat_count(3)   # Play 3 times then stop
```

**Jump around in the song:**
```gdscript
playback.set_position_order_row(4, 0)  # Jump to a specific part
print("Now at pattern: ", playback.get_current_pattern())
```

### Get Info About the Music

```gdscript
print("Song title: ", stream.get_title())
print("Made by: ", stream.get_artist())
print("Length: ", stream.get_length(), " seconds")
print("Instruments: ", stream.get_num_instruments())

# List all instruments in the song
var instruments = stream.get_instrument_names()
for i in range(instruments.size()):
    print(i, ": ", instruments[i])
```

## Where to Find Tracker Music

- [Mod Archive](https://modarchive.org/) - Huge collection of free tracker music
- [The Mod Archive API](https://modarchive.org/index.php?request=view_by_moduleid&query=176219) - Random module feature
- Your own creations using [OpenMPT](https://openmpt.org/) or [MilkyTracker](https://milkytracker.org/)

## Full Documentation

For complete API reference and advanced features, check the [documentation](https://github.com/RevoluPowered/libopenmpt-godot/tree/dev/docs)

## For Developers

### Running Tests

```bash
docker build -f tests/Dockerfile -t libopenmpt-tests .
docker run --rm libopenmpt-tests
```

Or manually:

```bash
scons tests=yes target=template_debug
./tests/run_tests
```

### Building Documentation

```bash
cd docs
docker-compose up --build
# Or: pip install -r requirements.txt && make html
```

## Contributing

Found a bug? Want to add a feature?

1. Fork this repo
2. Make your changes
3. Test it: `scons tests=yes && ./tests/run_tests`
4. Submit a pull request

## What File Formats Work?

This plugin supports 50+ tracker formats through libopenmpt:

**Popular formats:**
- `.mod` - Original Amiga ProTracker
- `.xm` - FastTracker II
- `.s3m` - ScreamTracker 3
- `.it` - Impulse Tracker
- `.mptm` - OpenMPT

**And many more:** `.669`, `.amf`, `.ams`, `.dbm`, `.digi`, `.dmf`, `.dsm`, `.dtm`, `.far`, `.gdm`, `.ice`, `.imf`, `.j2b`, `.m15`, `.mdl`, `.med`, `.mo3`, `.mt2`, `.mtm`, `.okt`, `.plm`, `.psm`, `.ptm`, `.sfx`, `.st26`, `.stk`, `.stm`, `.ult`, `.umx`, `.wow`

See the [libopenmpt website](https://lib.openmpt.org/libopenmpt/) for the complete list.

## License

This plugin is licensed under the MIT License - see the [LICENSE](LICENSE) file.

This project uses libopenmpt, which is licensed under the BSD license. See the [libopenmpt repository](https://github.com/OpenMPT/openmpt) for details.
