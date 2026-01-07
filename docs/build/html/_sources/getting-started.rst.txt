Getting Started
===============

Installation
------------

1. Clone the repository with submodules:

.. code-block:: bash

   git clone --recursive https://github.com/yourusername/libopenmpt-godot.git

2. Build the extension:

.. code-block:: bash

   cd libopenmpt-godot
   scons target=template_release

3. Copy the ``demo/bin`` folder to your Godot project

4. The extension will be automatically loaded by Godot when you open your project

Basic Usage
-----------

Loading and Playing Music
^^^^^^^^^^^^^^^^^^^^^^^^^^

The simplest way to play tracker music is to use the ``AudioStreamOpenMPT`` class with Godot's built-in ``AudioStreamPlayer``:

.. code-block:: gdscript

   extends Node

   func _ready():
       # Create the stream
       var stream = AudioStreamOpenMPT.new()

       # Load a module file
       var error = stream.load_from_file("res://music/song.mod")
       if error != OK:
           push_error("Failed to load module")
           return

       # Create and configure player
       var player = AudioStreamPlayer.new()
       add_child(player)
       player.stream = stream
       player.play()

Loading from Memory
^^^^^^^^^^^^^^^^^^^

You can also load module data directly from a ``PackedByteArray``:

.. code-block:: gdscript

   func load_from_bytes():
       var file = FileAccess.open("res://music/song.xm", FileAccess.READ)
       if file:
           var data = file.get_buffer(file.get_length())
           file.close()

           var stream = AudioStreamOpenMPT.new()
           stream.load_from_data(data)
           return stream
       return null

Accessing Module Information
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Once a module is loaded, you can access its metadata and structure:

.. code-block:: gdscript

   func print_module_info(stream: AudioStreamOpenMPT):
       print("Title: ", stream.get_title())
       print("Artist: ", stream.get_artist())
       print("Duration: ", stream.get_length(), " seconds")
       print("Channels: ", stream.get_num_channels())
       print("Patterns: ", stream.get_num_patterns())
       print("Instruments: ", stream.get_num_instruments())

       # List all instruments
       var instruments = stream.get_instrument_names()
       for i in range(instruments.size()):
           print("  ", i, ": ", instruments[i])

Controlling Playback
^^^^^^^^^^^^^^^^^^^^

For advanced playback control, access the ``AudioStreamPlaybackOpenMPT`` object:

.. code-block:: gdscript

   func setup_player(player: AudioStreamPlayer):
       # Get the playback object
       var playback = player.get_stream_playback() as AudioStreamPlaybackOpenMPT
       if not playback:
           return

       # Set infinite looping
       playback.set_repeat_count(-1)

       # Speed up playback by 50%
       playback.set_tempo_factor(1.5)

       # Pitch up
       playback.set_pitch_factor(1.2)

Next Steps
----------

* See :doc:`api-reference` for complete API documentation
* Check :doc:`examples` for more usage examples
* Read :doc:`building` for detailed build instructions
