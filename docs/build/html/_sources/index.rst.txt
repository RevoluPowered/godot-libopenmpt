libopenmpt-godot Documentation
==============================

A Godot GDExtension wrapper for libopenmpt, enabling playback of tracker music formats (MOD, XM, S3M, IT, and many more).

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   getting-started
   api-reference
   examples
   building
   testing

Features
--------

* Full support for tracker music formats via libopenmpt
* Native Godot AudioStream integration
* Real-time tempo and pitch control
* Pattern navigation and module introspection
* Comprehensive metadata access
* Cross-platform support (Windows, Linux, macOS)

Quick Start
-----------

.. code-block:: gdscript

   # Create and load a module
   var stream = AudioStreamOpenMPT.new()
   stream.load_from_file("res://music/song.mod")

   # Play using AudioStreamPlayer
   var player = AudioStreamPlayer.new()
   add_child(player)
   player.stream = stream
   player.play()

Indices and tables
==================

* :ref:`genindex`
* :ref:`search`
