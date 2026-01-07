API Reference
=============

AudioStreamOpenMPT
------------------

Main resource class for tracker modules. Extends Godot's ``AudioStream`` class.

Properties
^^^^^^^^^^

.. py:attribute:: data
   :type: PackedByteArray

   The raw module file data. Setting this property will attempt to load the module.

.. py:attribute:: mix_rate
   :type: int

   Sample rate for playback in Hz. Default is 48000.

Methods
^^^^^^^

Loading
"""""""

.. py:method:: load_from_file(path: String) -> Error

   Load a module from a file path.

   :param path: Path to the module file (e.g., "res://music/song.mod")
   :return: OK on success, or an error code
   :rtype: Error

   Example:

   .. code-block:: gdscript

      var stream = AudioStreamOpenMPT.new()
      var error = stream.load_from_file("res://music/song.mod")
      if error == OK:
          print("Module loaded successfully!")

.. py:method:: load_from_data(data: PackedByteArray) -> Error

   Load a module from raw byte data.

   :param data: Module file data as PackedByteArray
   :return: OK on success, ERR_INVALID_DATA if the data is invalid
   :rtype: Error

Metadata
""""""""

.. py:method:: get_title() -> String

   Get the module title from metadata.

   :return: Module title, or empty string if not available

.. py:method:: get_artist() -> String

   Get the artist/author name from metadata.

   :return: Artist name, or empty string if not available

.. py:method:: get_message() -> String

   Get the module message/comments.

   :return: Module message, or empty string if not available

.. py:method:: get_length() -> float

   Get the module duration in seconds.

   :return: Duration in seconds

.. py:method:: get_bpm() -> float

   Get the current tempo (beats per minute).

   :return: BPM value

Module Structure
""""""""""""""""

.. py:method:: get_num_channels() -> int

   Get the number of channels in the module.

   :return: Number of channels

.. py:method:: get_num_orders() -> int

   Get the number of orders in the sequence.

   :return: Number of orders

.. py:method:: get_num_patterns() -> int

   Get the number of patterns.

   :return: Number of patterns

.. py:method:: get_num_instruments() -> int

   Get the number of instruments.

   :return: Number of instruments

.. py:method:: get_num_samples() -> int

   Get the number of samples.

   :return: Number of samples

Names and Lists
"""""""""""""""

.. py:method:: get_channel_names() -> PackedStringArray

   Get all channel names.

   :return: Array of channel names

.. py:method:: get_order_names() -> PackedStringArray

   Get all order names.

   :return: Array of order names

.. py:method:: get_pattern_names() -> PackedStringArray

   Get all pattern names.

   :return: Array of pattern names

.. py:method:: get_instrument_names() -> PackedStringArray

   Get all instrument names.

   :return: Array of instrument names

   Example:

   .. code-block:: gdscript

      var instruments = stream.get_instrument_names()
      for i in range(instruments.size()):
          print("Instrument ", i, ": ", instruments[i])

.. py:method:: get_sample_names() -> PackedStringArray

   Get all sample names.

   :return: Array of sample names

AudioStreamPlaybackOpenMPT
---------------------------

Playback control class. Extends Godot's ``AudioStreamPlayback`` class. Access this via ``AudioStreamPlayer.get_stream_playback()``.

Methods
^^^^^^^

Playback Control
""""""""""""""""

.. py:method:: set_position(position: float) -> void

   Seek to a specific position in seconds.

   :param position: Position in seconds

.. py:method:: get_position() -> float

   Get the current playback position in seconds.

   :return: Current position

.. py:method:: set_repeat_count(count: int) -> void

   Set the loop count.

   :param count: Number of times to repeat (-1 for infinite loop, 0 for no repeat)

.. py:method:: get_repeat_count() -> int

   Get the current loop count setting.

   :return: Loop count

Real-time Effects
"""""""""""""""""

.. py:method:: set_tempo_factor(factor: float) -> void

   Change the playback speed without affecting pitch.

   :param factor: Tempo multiplier (1.0 = normal, 2.0 = double speed, 0.5 = half speed)

   Example:

   .. code-block:: gdscript

      var playback = player.get_stream_playback() as AudioStreamPlaybackOpenMPT
      playback.set_tempo_factor(1.5)  # Play 50% faster

.. py:method:: get_tempo_factor() -> float

   Get the current tempo factor.

   :return: Current tempo multiplier

.. py:method:: set_pitch_factor(factor: float) -> void

   Change the pitch without affecting tempo.

   :param factor: Pitch multiplier (1.0 = normal, 2.0 = one octave up, 0.5 = one octave down)

.. py:method:: get_pitch_factor() -> float

   Get the current pitch factor.

   :return: Current pitch multiplier

Pattern Navigation
""""""""""""""""""

.. py:method:: get_current_order() -> int

   Get the current order index in the sequence.

   :return: Current order index

.. py:method:: get_current_pattern() -> int

   Get the current pattern index.

   :return: Current pattern index

.. py:method:: get_current_row() -> int

   Get the current row within the pattern.

   :return: Current row

.. py:method:: set_position_order_row(order: int, row: int) -> void

   Jump to a specific order and row.

   :param order: Order index to jump to
   :param row: Row within the pattern

   Example:

   .. code-block:: gdscript

      var playback = player.get_stream_playback() as AudioStreamPlaybackOpenMPT
      playback.set_position_order_row(4, 0)  # Jump to order 4, row 0

Inherited Methods
^^^^^^^^^^^^^^^^^

From AudioStreamPlayback:

* ``_start(from_pos: float)`` - Start playback
* ``_stop()`` - Stop playback
* ``_is_playing()`` - Check if playing
* ``_get_loop_count()`` - Get number of loops completed
* ``_seek(time: float)`` - Seek to time position
