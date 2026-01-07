extends SceneTree

# GDScript test runner that executes C++ doctest tests within Godot runtime
# This ensures ClassDB and other Godot systems are properly initialized

func _init():
	print("=== libopenmpt-godot Test Runner ===")
	print("Godot runtime initialized, running C++ unit tests...\n")

	# Create TestRunner instance (available when tests=yes is built)
	var runner = TestRunner.new()

	# Run tests
	var exit_code = runner.run_tests()

	print("\n=== Test Execution Complete ===")
	print("Exit code: ", exit_code)

	# Exit with the test result code
	quit(exit_code)
