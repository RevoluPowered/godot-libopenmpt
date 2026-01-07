Building from Source
====================

Prerequisites
-------------

Required Tools
^^^^^^^^^^^^^^

* **Git** - For cloning the repository and submodules
* **Python 3.6+** - Required by SCons
* **SCons 4.0+** - Build system
* **C++17 Compiler** - One of:

  * GCC 7+ (Linux)
  * Clang 6+ (macOS, Linux)
  * MSVC 2017+ (Windows)
  * MinGW-w64 (Windows alternative)

Platform-Specific Requirements
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**Linux:**

.. code-block:: bash

   # Debian/Ubuntu
   sudo apt install build-essential python3 python3-pip git

   # Install SCons
   pip3 install scons

**macOS:**

.. code-block:: bash

   # Install Xcode Command Line Tools
   xcode-select --install

   # Install SCons
   pip3 install scons

**Windows:**

* Install Visual Studio 2017 or later with C++ support
* Install Python from python.org
* Install SCons: ``pip install scons``
* Or use MinGW-w64 as an alternative compiler

Clone the Repository
---------------------

Clone with all submodules:

.. code-block:: bash

   git clone --recursive https://github.com/yourusername/libopenmpt-godot.git
   cd libopenmpt-godot

If you already cloned without ``--recursive``, initialize submodules:

.. code-block:: bash

   git submodule update --init --recursive

Build the Extension
-------------------

Basic Build
^^^^^^^^^^^

Build a debug version:

.. code-block:: bash

   scons target=template_debug

Build a release version:

.. code-block:: bash

   scons target=template_release

The compiled library will be placed in ``demo/bin/``.

Platform-Specific Builds
^^^^^^^^^^^^^^^^^^^^^^^^^

Build for specific platform:

.. code-block:: bash

   # macOS
   scons platform=macos target=template_release

   # Windows (64-bit)
   scons platform=windows target=template_release

   # Linux (64-bit)
   scons platform=linux target=template_release

Multi-threaded Build
^^^^^^^^^^^^^^^^^^^^

Use multiple CPU cores for faster compilation:

.. code-block:: bash

   scons -j8 target=template_release

Replace ``8`` with your CPU core count.

Build Options
-------------

SCons Command Line Options
^^^^^^^^^^^^^^^^^^^^^^^^^^^

* ``target=<template_debug|template_release>`` - Build type
* ``platform=<windows|macos|linux>`` - Target platform
* ``arch=<x86_64|arm64>`` - Target architecture
* ``-j<N>`` - Use N parallel build jobs
* ``-c`` - Clean build artifacts
* ``tests=yes`` - Build unit tests (see below)

Clean Build
^^^^^^^^^^^

Remove all build artifacts:

.. code-block:: bash

   scons -c

Building Unit Tests
-------------------

Build the test executable:

.. code-block:: bash

   scons tests=yes target=template_debug

Run the tests:

.. code-block:: bash

   ./tests/run_tests

The tests use the doctest framework and will output detailed results.

Cross-Compilation
-----------------

macOS Universal Binary
^^^^^^^^^^^^^^^^^^^^^^

Build for both Intel and Apple Silicon:

.. code-block:: bash

   scons platform=macos arch=universal target=template_release

Windows Cross-Compilation from Linux
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Install MinGW-w64:

.. code-block:: bash

   sudo apt install mingw-w64

Build for Windows:

.. code-block:: bash

   scons platform=windows target=template_release use_mingw=yes

Integration with Godot
----------------------

After Building
^^^^^^^^^^^^^^

1. The compiled library and ``.gdextension`` file are in ``demo/bin/``
2. Copy the entire ``demo/bin`` folder to your Godot project
3. Godot will automatically detect and load the extension

Verify Installation
^^^^^^^^^^^^^^^^^^^

In Godot's script editor, check if the classes are available:

.. code-block:: gdscript

   func _ready():
       var stream = AudioStreamOpenMPT.new()
       print("Extension loaded: ", stream != null)

Troubleshooting
---------------

Common Issues
^^^^^^^^^^^^^

**"scons: command not found"**

Install SCons:

.. code-block:: bash

   pip3 install scons

**Compilation errors about missing godot-cpp**

Initialize submodules:

.. code-block:: bash

   git submodule update --init --recursive

**Errors about libopenmpt headers**

Make sure the libopenmpt submodule is properly initialized:

.. code-block:: bash

   ls libopenmpt/libopenmpt/libopenmpt.hpp

If the file doesn't exist, run:

.. code-block:: bash

   git submodule update --init --recursive

**macOS: "cannot code sign" errors**

Add to your build command:

.. code-block:: bash

   scons target=template_release macos_disable_exception_handling=no

**Windows: "MSVC not found"**

Ensure Visual Studio C++ tools are installed, or use MinGW:

.. code-block:: bash

   scons platform=windows use_mingw=yes

Build Configuration
-------------------

Custom Build Settings
^^^^^^^^^^^^^^^^^^^^^

Create a ``custom.py`` file in the project root to customize build settings:

.. code-block:: python

   # custom.py
   import os

   # Custom defines
   CPPDEFINES = ["MY_CUSTOM_DEFINE"]

   # Custom include paths
   CPPPATH = ["/path/to/custom/includes"]

   # Custom library paths
   LIBPATH = ["/path/to/custom/libs"]

This file is automatically loaded by SCons if it exists.

Development Builds
^^^^^^^^^^^^^^^^^^

For development with debug symbols and faster compilation:

.. code-block:: bash

   scons target=template_debug dev_build=yes

This enables:

* Debug symbols
* Less optimization
* Faster incremental builds
* More verbose output

Debugging the Extension
-----------------------

Debug Build
^^^^^^^^^^^

Always use debug builds when developing:

.. code-block:: bash

   scons target=template_debug

Use with Godot's Debug Build
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

1. Download Godot debug build
2. Run from terminal to see output:

.. code-block:: bash

   ./Godot.app/Contents/MacOS/Godot --verbose

GDB/LLDB Debugging
^^^^^^^^^^^^^^^^^^

**Linux (GDB):**

.. code-block:: bash

   gdb godot
   (gdb) run --path /path/to/project

**macOS (LLDB):**

.. code-block:: bash

   lldb Godot.app/Contents/MacOS/Godot
   (lldb) run -- --path /path/to/project

Contributing
------------

When submitting changes:

1. Ensure both debug and release builds compile
2. Run unit tests: ``scons tests=yes && ./tests/run_tests``
3. Test on your target platforms
4. Follow existing code style

See the project's CONTRIBUTING.md for detailed guidelines.
