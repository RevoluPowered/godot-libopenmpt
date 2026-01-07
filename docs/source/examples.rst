Examples
========

Basic Music Player
------------------

A simple music player with play/pause/stop controls:

.. code-block:: gdscript

   extends Control

   var player: AudioStreamPlayer
   var stream: AudioStreamOpenMPT

   func _ready():
       # Setup player
       player = AudioStreamPlayer.new()
       add_child(player)

       # Load module
       stream = AudioStreamOpenMPT.new()
       if stream.load_from_file("res://music/song.mod") == OK:
           player.stream = stream
           print("Loaded: ", stream.get_title())
       else:
           push_error("Failed to load module")

   func play_music():
       player.play()

   func pause_music():
       player.stream_paused = !player.stream_paused

   func stop_music():
       player.stop()

Module Information Display
--------------------------

Display detailed information about a loaded module:

.. code-block:: gdscript

   extends Node

   func display_module_info(stream: AudioStreamOpenMPT):
       print("=" * 50)
       print("MODULE INFORMATION")
       print("=" * 50)

       # Metadata
       print("Title:    ", stream.get_title())
       print("Artist:   ", stream.get_artist())
       print("Duration: ", "%.2f" % stream.get_length(), " seconds")
       print("BPM:      ", stream.get_bpm())

       # Structure
       print("\nSTRUCTURE:")
       print("Channels:    ", stream.get_num_channels())
       print("Orders:      ", stream.get_num_orders())
       print("Patterns:    ", stream.get_num_patterns())
       print("Instruments: ", stream.get_num_instruments())
       print("Samples:     ", stream.get_num_samples())

       # List channels
       print("\nCHANNELS:")
       var channels = stream.get_channel_names()
       for i in range(channels.size()):
           print("  %2d: %s" % [i, channels[i]])

       # List instruments
       print("\nINSTRUMENTS:")
       var instruments = stream.get_instrument_names()
       for i in range(instruments.size()):
           if instruments[i] != "":
               print("  %2d: %s" % [i, instruments[i]])

       # Module message
       var message = stream.get_message()
       if message != "":
           print("\nMESSAGE:")
           print(message)

Interactive Music Player
------------------------

A player with tempo and pitch control:

.. code-block:: gdscript

   extends Control

   @onready var player = $AudioStreamPlayer
   @onready var tempo_slider = $TempoSlider
   @onready var pitch_slider = $PitchSlider
   @onready var position_label = $PositionLabel

   func _ready():
       var stream = AudioStreamOpenMPT.new()
       stream.load_from_file("res://music/song.mod")
       player.stream = stream
       player.play()

       # Setup sliders
       tempo_slider.min_value = 0.5
       tempo_slider.max_value = 2.0
       tempo_slider.value = 1.0
       tempo_slider.value_changed.connect(_on_tempo_changed)

       pitch_slider.min_value = 0.5
       pitch_slider.max_value = 2.0
       pitch_slider.value = 1.0
       pitch_slider.value_changed.connect(_on_pitch_changed)

   func _process(_delta):
       # Update position display
       var playback = player.get_stream_playback() as AudioStreamPlaybackOpenMPT
       if playback:
           var pos = playback.get_position()
           var order = playback.get_current_order()
           var pattern = playback.get_current_pattern()
           var row = playback.get_current_row()

           position_label.text = "Time: %.2fs | Order: %d | Pattern: %d | Row: %d" % [
               pos, order, pattern, row
           ]

   func _on_tempo_changed(value: float):
       var playback = player.get_stream_playback() as AudioStreamPlaybackOpenMPT
       if playback:
           playback.set_tempo_factor(value)

   func _on_pitch_changed(value: float):
       var playback = player.get_stream_playback() as AudioStreamPlaybackOpenMPT
       if playback:
           playback.set_pitch_factor(value)

Pattern-based Music System
---------------------------

Use pattern navigation for dynamic music:

.. code-block:: gdscript

   extends Node

   var player: AudioStreamPlayer
   var playback: AudioStreamPlaybackOpenMPT

   # Music intensity levels mapped to orders
   const MUSIC_CALM = 0
   const MUSIC_MEDIUM = 4
   const MUSIC_INTENSE = 8

   func _ready():
       player = AudioStreamPlayer.new()
       add_child(player)

       var stream = AudioStreamOpenMPT.new()
       stream.load_from_file("res://music/dynamic_soundtrack.mod")
       player.stream = stream

       playback = player.get_stream_playback() as AudioStreamPlaybackOpenMPT
       playback.set_repeat_count(-1)  # Loop forever

       player.play()
       set_music_intensity(MUSIC_CALM)

   func set_music_intensity(order: int):
       """Change music intensity by jumping to different order"""
       if playback:
           playback.set_position_order_row(order, 0)
           print("Music intensity changed to order ", order)

   # Example: Change intensity based on game state
   func _on_enemy_spawned():
       set_music_intensity(MUSIC_MEDIUM)

   func _on_boss_fight_started():
       set_music_intensity(MUSIC_INTENSE)

   func _on_battle_ended():
       set_music_intensity(MUSIC_CALM)

Module Browser
--------------

Browse and play modules from a directory:

.. code-block:: gdscript

   extends Control

   @onready var file_list = $FileList
   @onready var player = $AudioStreamPlayer
   @onready var info_label = $InfoLabel

   var modules_dir = "res://music/"
   var current_stream: AudioStreamOpenMPT

   func _ready():
       load_module_list()

   func load_module_list():
       var dir = DirAccess.open(modules_dir)
       if dir:
           dir.list_dir_begin()
           var file_name = dir.get_next()

           while file_name != "":
               if not dir.current_is_dir():
                   if is_tracker_file(file_name):
                       file_list.add_item(file_name)
               file_name = dir.get_next()

           dir.list_dir_end()

   func is_tracker_file(filename: String) -> bool:
       var ext = filename.get_extension().to_lower()
       return ext in ["mod", "xm", "s3m", "it", "mptm"]

   func _on_file_list_item_selected(index: int):
       var filename = file_list.get_item_text(index)
       load_and_play_module(modules_dir + filename)

   func load_and_play_module(path: String):
       current_stream = AudioStreamOpenMPT.new()

       if current_stream.load_from_file(path) == OK:
           player.stream = current_stream
           player.play()

           # Update info display
           info_label.text = """
           Title: %s
           Artist: %s
           Duration: %.2f seconds
           Channels: %d
           Instruments: %d
           """ % [
               current_stream.get_title(),
               current_stream.get_artist(),
               current_stream.get_length(),
               current_stream.get_num_channels(),
               current_stream.get_num_instruments()
           ]
       else:
           info_label.text = "Failed to load: " + path

Random Module Player
--------------------

Randomly select and play modules with crossfade:

.. code-block:: gdscript

   extends Node

   var player1: AudioStreamPlayer
   var player2: AudioStreamPlayer
   var current_player: AudioStreamPlayer
   var next_player: AudioStreamPlayer

   var module_paths: Array[String] = [
       "res://music/song1.mod",
       "res://music/song2.xm",
       "res://music/song3.s3m",
   ]

   func _ready():
       # Setup two players for crossfading
       player1 = AudioStreamPlayer.new()
       player2 = AudioStreamPlayer.new()
       add_child(player1)
       add_child(player2)

       current_player = player1
       next_player = player2

       play_random_module()

   func play_random_module():
       var path = module_paths[randi() % module_paths.size()]

       var stream = AudioStreamOpenMPT.new()
       if stream.load_from_file(path) == OK:
           current_player.stream = stream
           current_player.play()

           print("Now playing: ", stream.get_title())

           # Schedule next song
           var duration = stream.get_length()
           get_tree().create_timer(duration - 2.0).timeout.connect(_on_song_ending)

   func _on_song_ending():
       # Start crossfade to next song
       start_crossfade()

   func start_crossfade():
       # Load next song into next_player
       var path = module_paths[randi() % module_paths.size()]
       var stream = AudioStreamOpenMPT.new()

       if stream.load_from_file(path) == OK:
           next_player.stream = stream
           next_player.volume_db = -80
           next_player.play()

           # Crossfade
           var tween = create_tween()
           tween.set_parallel(true)
           tween.tween_property(current_player, "volume_db", -80, 2.0)
           tween.tween_property(next_player, "volume_db", 0, 2.0)
           tween.finished.connect(_on_crossfade_complete)

   func _on_crossfade_complete():
       current_player.stop()
       # Swap players
       var temp = current_player
       current_player = next_player
       next_player = temp

       # Schedule next transition
       var stream = current_player.stream as AudioStreamOpenMPT
       var duration = stream.get_length()
       get_tree().create_timer(duration - 2.0).timeout.connect(_on_song_ending)
