extends VestTest

func get_suite_name() -> String:
	return "AudioStreamOpenMPT Integration"

func test_load_mod_from_file():
	var stream = AudioStreamOpenMPT.new()
	var err = stream.load_from_file("res://tests/fixtures/crazy_heavy_crash_boom.mod")
	expect_equal(err, OK)
	expect_not_empty(stream.get_title())
	expect(stream.get_num_channels() > 0, "Module should have channels")
	expect(stream.get_num_patterns() > 0, "Module should have patterns")
	expect(stream.get_num_samples() > 0, "Module should have samples")
	expect(stream.get_length() > 0.0, "Module should have nonzero length")

func test_load_mod_from_data():
	var file = FileAccess.open("res://tests/fixtures/crazy_heavy_crash_boom.mod", FileAccess.READ)
	expect_not_null(file)
	var data = file.get_buffer(file.get_length())
	file.close()

	var stream = AudioStreamOpenMPT.new()
	var err = stream.load_from_data(data)
	expect_equal(err, OK)
	expect_not_empty(stream.get_title())

func test_mod_metadata():
	var stream = AudioStreamOpenMPT.new()
	stream.load_from_file("res://tests/fixtures/crazy_heavy_crash_boom.mod")
	# MOD files have 4 channels
	expect_equal(stream.get_num_channels(), 4)
	expect(stream.get_sample_names().size() > 0, "Should have sample names")

func test_instantiate_playback():
	var stream = AudioStreamOpenMPT.new()
	stream.load_from_file("res://tests/fixtures/crazy_heavy_crash_boom.mod")
	var playback = stream.instantiate_playback()
	expect_not_null(playback)
