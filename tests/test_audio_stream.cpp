#include "doctest/doctest/doctest.h"
#include "../src/audio_stream_openmpt.h"
#include <godot_cpp/classes/file_access.hpp>

using namespace godot;

TEST_SUITE("AudioStreamOpenMPT") {
    TEST_CASE("Constructor initializes default values") {
        Ref<AudioStreamOpenMPT> stream;
        stream.instantiate();

        CHECK(stream.is_valid());
        CHECK(stream->get_data().size() == 0);
        CHECK(stream->get_mix_rate() == 48000);
        CHECK(stream->get_length() == 0.0);
    }

    TEST_CASE("Mix rate can be set and retrieved") {
        Ref<AudioStreamOpenMPT> stream;
        stream.instantiate();

        stream->set_mix_rate(44100);
        CHECK(stream->get_mix_rate() == 44100);

        stream->set_mix_rate(96000);
        CHECK(stream->get_mix_rate() == 96000);
    }

    TEST_CASE("Empty data returns empty metadata") {
        Ref<AudioStreamOpenMPT> stream;
        stream.instantiate();

        CHECK(stream->get_title() == "");
        CHECK(stream->get_artist() == "");
        CHECK(stream->get_message() == "");
        CHECK(stream->get_num_channels() == 0);
        CHECK(stream->get_num_orders() == 0);
        CHECK(stream->get_num_patterns() == 0);
        CHECK(stream->get_num_instruments() == 0);
        CHECK(stream->get_num_samples() == 0);
    }

    TEST_CASE("Empty data returns empty name arrays") {
        Ref<AudioStreamOpenMPT> stream;
        stream.instantiate();

        CHECK(stream->get_channel_names().size() == 0);
        CHECK(stream->get_order_names().size() == 0);
        CHECK(stream->get_pattern_names().size() == 0);
        CHECK(stream->get_instrument_names().size() == 0);
        CHECK(stream->get_sample_names().size() == 0);
    }

    TEST_CASE("load_from_data with empty data returns error") {
        Ref<AudioStreamOpenMPT> stream;
        stream.instantiate();

        PackedByteArray empty_data;
        Error result = stream->load_from_data(empty_data);

        CHECK(result == ERR_INVALID_DATA);
    }

    TEST_CASE("load_from_data with invalid data returns error") {
        Ref<AudioStreamOpenMPT> stream;
        stream.instantiate();

        PackedByteArray invalid_data;
        invalid_data.resize(100);
        for (int i = 0; i < 100; i++) {
            invalid_data[i] = i;
        }

        Error result = stream->load_from_data(invalid_data);
        CHECK(result == ERR_INVALID_DATA);
    }

    TEST_CASE("Stream is not monophonic") {
        Ref<AudioStreamOpenMPT> stream;
        stream.instantiate();

        CHECK(stream->_is_monophonic() == false);
    }

    TEST_CASE("Stream name returns default when no data loaded") {
        Ref<AudioStreamOpenMPT> stream;
        stream.instantiate();

        String name = stream->_get_stream_name();
        CHECK(name == "AudioStreamOpenMPT");
    }
}

TEST_SUITE("AudioStreamPlaybackOpenMPT") {
    TEST_CASE("Constructor initializes default values") {
        Ref<AudioStreamPlaybackOpenMPT> playback;
        playback.instantiate();

        CHECK(playback.is_valid());
        CHECK(playback->get_position() == 0.0);
        CHECK(playback->_is_playing() == false);
        CHECK(playback->_get_loop_count() == 0);
    }

    TEST_CASE("Playback position can be set") {
        Ref<AudioStreamPlaybackOpenMPT> playback;
        playback.instantiate();

        playback->set_position(5.0);
        // Position might not change without a valid module, but shouldn't crash
        CHECK(playback.is_valid());
    }

    TEST_CASE("Initially not playing") {
        Ref<AudioStreamPlaybackOpenMPT> playback;
        playback.instantiate();

        CHECK(playback->_is_playing() == false);
    }

    TEST_CASE("Stop when not playing doesn't crash") {
        Ref<AudioStreamPlaybackOpenMPT> playback;
        playback.instantiate();

        playback->_stop();
        CHECK(playback->_is_playing() == false);
    }

    TEST_CASE("Pattern navigation methods don't crash without module") {
        Ref<AudioStreamPlaybackOpenMPT> playback;
        playback.instantiate();

        CHECK(playback->get_current_order() == 0);
        CHECK(playback->get_current_pattern() == 0);
        CHECK(playback->get_current_row() == 0);

        // Should not crash
        playback->set_position_order_row(0, 0);
        CHECK(playback.is_valid());
    }

    TEST_CASE("Tempo factor methods don't crash without module") {
        Ref<AudioStreamPlaybackOpenMPT> playback;
        playback.instantiate();

        playback->set_tempo_factor(1.5);
        double factor = playback->get_tempo_factor();
        CHECK(factor >= 0.0);  // Should return valid value
    }

    TEST_CASE("Pitch factor methods don't crash without module") {
        Ref<AudioStreamPlaybackOpenMPT> playback;
        playback.instantiate();

        playback->set_pitch_factor(1.2);
        double factor = playback->get_pitch_factor();
        CHECK(factor >= 0.0);  // Should return valid value
    }

    TEST_CASE("Repeat count methods don't crash without module") {
        Ref<AudioStreamPlaybackOpenMPT> playback;
        playback.instantiate();

        playback->set_repeat_count(-1);
        int count = playback->get_repeat_count();
        CHECK(count >= -1);  // Should return valid value
    }

    TEST_CASE("Mix with null buffer returns 0") {
        Ref<AudioStreamPlaybackOpenMPT> playback;
        playback.instantiate();

        int frames = playback->_mix(nullptr, 1.0, 0);
        CHECK(frames == 0);
    }
}

TEST_SUITE("Integration Tests") {
    TEST_CASE("Instantiate playback from stream") {
        Ref<AudioStreamOpenMPT> stream;
        stream.instantiate();

        Ref<AudioStreamPlayback> playback = stream->_instantiate_playback();
        CHECK(playback.is_valid());

        // Should be able to cast to AudioStreamPlaybackOpenMPT
        Ref<AudioStreamPlaybackOpenMPT> openmpt_playback = playback;
        CHECK(openmpt_playback.is_valid());
    }
}
