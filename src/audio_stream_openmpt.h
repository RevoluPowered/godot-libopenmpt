#ifndef AUDIO_STREAM_OPENMPT_H
#define AUDIO_STREAM_OPENMPT_H

#include <godot_cpp/classes/audio_stream.hpp>
#include <godot_cpp/classes/audio_stream_playback.hpp>
#include <godot_cpp/classes/ref.hpp>

// Forward declare C API types
struct openmpt_module;

namespace godot {

class AudioStreamPlaybackOpenMPT;

class AudioStreamOpenMPT : public AudioStream {
    GDCLASS(AudioStreamOpenMPT, AudioStream)

    friend class AudioStreamPlaybackOpenMPT;

private:
    PackedByteArray data;
    String file_path;
    double length;
    int mix_rate;

protected:
    static void _bind_methods();

public:
    AudioStreamOpenMPT();
    ~AudioStreamOpenMPT();

    // AudioStream virtuals
    Ref<AudioStreamPlayback> _instantiate_playback() const override;
    String _get_stream_name() const override;
    double _get_length() const override;
    bool _is_monophonic() const override;
    double _get_bpm() const override;
    int _get_beat_count() const override;

    // Custom methods
    Error load_from_file(const String &p_path);
    Error load_from_data(const PackedByteArray &p_data);

    void set_data(const PackedByteArray &p_data);
    PackedByteArray get_data() const;

    void set_mix_rate(int p_mix_rate);
    int get_mix_rate() const;

    // Module information
    String get_title() const;
    String get_artist() const;
    String get_message() const;
    int get_num_channels() const;
    int get_num_orders() const;
    int get_num_patterns() const;
    int get_num_instruments() const;
    int get_num_samples() const;
    PackedStringArray get_channel_names() const;
    PackedStringArray get_order_names() const;
    PackedStringArray get_pattern_names() const;
    PackedStringArray get_instrument_names() const;
    PackedStringArray get_sample_names() const;
};

class AudioStreamPlaybackOpenMPT : public AudioStreamPlayback {
    GDCLASS(AudioStreamPlaybackOpenMPT, AudioStreamPlayback)

    friend class AudioStreamOpenMPT;

private:
    Ref<AudioStreamOpenMPT> stream;
    openmpt_module* module;
    double position;
    bool active;
    int loops;

    void _load_module();

protected:
    static void _bind_methods();

public:
    AudioStreamPlaybackOpenMPT();
    ~AudioStreamPlaybackOpenMPT();

    // AudioStreamPlayback virtuals
    void _start(double p_from_pos) override;
    void _stop() override;
    bool _is_playing() const override;
    int _get_loop_count() const override;
    double _get_playback_position() const override;
    void _seek(double p_time) override;
    int32_t _mix(AudioFrame *p_buffer, float p_rate_scale, int32_t p_frames) override;
    void _tag_used_streams() override;

    // Custom methods
    void set_stream(const Ref<AudioStreamOpenMPT> &p_stream);

    // Playback control
    void set_position(double p_position);
    double get_position() const;

    // Module control
    void set_repeat_count(int p_count);
    int get_repeat_count() const;

    void set_tempo_factor(double p_factor);
    double get_tempo_factor() const;

    void set_pitch_factor(double p_factor);
    double get_pitch_factor() const;

    // Pattern/Order navigation
    int get_current_order() const;
    int get_current_pattern() const;
    int get_current_row() const;

    void set_position_order_row(int p_order, int p_row);
};

}

#endif // AUDIO_STREAM_OPENMPT_H
