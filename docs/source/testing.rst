Testing
=======

The project uses doctest for C++ unit testing.

Running Tests
-------------

Build and run the test suite:

.. code-block:: bash

   # Build tests
   scons tests=yes target=template_debug

   # Run tests
   ./tests/run_tests

Test Output
-----------

Example successful test output:

.. code-block:: text

   [doctest] doctest version is "2.4.11"
   [doctest] run with "--help" for options
   ===============================================================================
   [doctest] test cases: 15 | 15 passed | 0 failed | 0 skipped
   [doctest] assertions: 45 | 45 passed | 0 failed |
   [doctest] Status: SUCCESS!

Test Organization
-----------------

Tests are organized by test suites:

* ``AudioStreamOpenMPT`` - Tests for the main stream class
* ``AudioStreamPlaybackOpenMPT`` - Tests for playback control
* ``Integration Tests`` - End-to-end functionality tests

Test Files
^^^^^^^^^^

* ``tests/test_main.cpp`` - Test runner entry point
* ``tests/test_audio_stream.cpp`` - AudioStream tests

Writing Tests
-------------

Basic Test Structure
^^^^^^^^^^^^^^^^^^^^

.. code-block:: cpp

   #include "doctest/doctest/doctest.h"
   #include "../src/audio_stream_openmpt.h"

   TEST_SUITE("MyTestSuite") {
       TEST_CASE("Test description") {
           // Arrange
           Ref<AudioStreamOpenMPT> stream;
           stream.instantiate();

           // Act
           stream->set_mix_rate(44100);

           // Assert
           CHECK(stream->get_mix_rate() == 44100);
       }
   }

Assertions
^^^^^^^^^^

doctest provides several assertion macros:

.. code-block:: cpp

   // Equality checks
   CHECK(value == expected);
   REQUIRE(value == expected);  // Stops test on failure

   // Boolean checks
   CHECK(stream.is_valid());
   CHECK_FALSE(stream.is_null());

   // Floating point comparison
   CHECK(value == doctest::Approx(3.14).epsilon(0.01));

   // Exception checking
   CHECK_THROWS(risky_operation());
   CHECK_NOTHROW(safe_operation());

Test Fixtures
^^^^^^^^^^^^^

Use subcases for setup/teardown:

.. code-block:: cpp

   TEST_CASE("Stream operations") {
       Ref<AudioStreamOpenMPT> stream;
       stream.instantiate();

       SUBCASE("Loading from file") {
           auto result = stream->load_from_file("test.mod");
           CHECK(result == OK);
       }

       SUBCASE("Loading from data") {
           PackedByteArray data;
           auto result = stream->load_from_data(data);
           CHECK(result == ERR_INVALID_DATA);
       }
   }

Testing with Module Files
--------------------------

For tests that require actual module files, place test modules in ``tests/data/``:

.. code-block:: cpp

   TEST_CASE("Load valid module") {
       Ref<AudioStreamOpenMPT> stream;
       stream.instantiate();

       Error err = stream->load_from_file("tests/data/test.mod");
       CHECK(err == OK);
       CHECK(stream->get_length() > 0.0);
   }

Continuous Integration
----------------------

GitHub Actions
^^^^^^^^^^^^^^

Example workflow file (``.github/workflows/tests.yml``):

.. code-block:: yaml

   name: Tests

   on: [push, pull_request]

   jobs:
     test:
       runs-on: ubuntu-latest

       steps:
       - uses: actions/checkout@v3
         with:
           submodules: recursive

       - name: Install dependencies
         run: |
           sudo apt-get update
           sudo apt-get install -y build-essential python3-pip
           pip3 install scons

       - name: Build tests
         run: scons tests=yes target=template_debug

       - name: Run tests
         run: ./tests/run_tests

Code Coverage
-------------

Generate coverage reports with gcov/lcov:

.. code-block:: bash

   # Build with coverage flags
   scons tests=yes target=template_debug coverage=yes

   # Run tests
   ./tests/run_tests

   # Generate coverage report
   lcov --capture --directory . --output-file coverage.info
   lcov --remove coverage.info '/usr/*' --output-file coverage.info
   genhtml coverage.info --output-directory coverage_report

Benchmarking
------------

Use doctest's benchmarking features:

.. code-block:: cpp

   TEST_CASE("Performance benchmark") {
       Ref<AudioStreamOpenMPT> stream;
       stream.instantiate();
       stream->load_from_file("tests/data/large.mod");

       auto playback = stream->_instantiate_playback();

       BENCHMARK("Audio mixing") {
           AudioFrame buffer[4096];
           return playback->_mix(buffer, 1.0, 4096);
       };
   }

Test Best Practices
-------------------

1. **Test Isolation**: Each test should be independent
2. **Clear Names**: Use descriptive test case names
3. **One Concept Per Test**: Test one thing at a time
4. **Fast Tests**: Keep tests quick to run
5. **Deterministic**: Tests should always produce the same result

Example Test Categories
-----------------------

Unit Tests
^^^^^^^^^^

Test individual methods in isolation:

.. code-block:: cpp

   TEST_CASE("get_mix_rate returns set value") {
       Ref<AudioStreamOpenMPT> stream;
       stream.instantiate();
       stream->set_mix_rate(96000);
       CHECK(stream->get_mix_rate() == 96000);
   }

Integration Tests
^^^^^^^^^^^^^^^^^

Test interaction between components:

.. code-block:: cpp

   TEST_CASE("Stream can create playback instance") {
       Ref<AudioStreamOpenMPT> stream;
       stream.instantiate();

       auto playback = stream->_instantiate_playback();
       CHECK(playback.is_valid());

       auto openmpt_playback = Object::cast_to<AudioStreamPlaybackOpenMPT>(playback.ptr());
       CHECK(openmpt_playback != nullptr);
   }

Edge Case Tests
^^^^^^^^^^^^^^^

Test boundary conditions and error cases:

.. code-block:: cpp

   TEST_CASE("Empty data returns error") {
       Ref<AudioStreamOpenMPT> stream;
       stream.instantiate();

       PackedByteArray empty;
       CHECK(stream->load_from_data(empty) == ERR_INVALID_DATA);
   }

   TEST_CASE("Invalid data returns error") {
       Ref<AudioStreamOpenMPT> stream;
       stream.instantiate();

       PackedByteArray invalid;
       invalid.resize(100);
       CHECK(stream->load_from_data(invalid) == ERR_INVALID_DATA);
   }

Debugging Failed Tests
----------------------

Run specific test:

.. code-block:: bash

   ./tests/run_tests --test-case="Test name"

Run specific test suite:

.. code-block:: bash

   ./tests/run_tests --test-suite="AudioStreamOpenMPT"

Verbose output:

.. code-block:: bash

   ./tests/run_tests --success

Break on failure:

.. code-block:: bash

   ./tests/run_tests --abort-after=1

List all tests:

.. code-block:: bash

   ./tests/run_tests --list-test-cases
