#include "audio_stream_openmpt.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <libopenmpt/libopenmpt.h>
#include <cstring>

using namespace godot;

// Helper function to safely create module from data
static openmpt_module* safe_create_module(const uint8_t* data, size_t size) {
    if (!data || size == 0) {
        return nullptr;
    }
    return openmpt_module_create_from_memory(data, size, nullptr, nullptr, nullptr);
}

// AudioStreamOpenMPT

void AudioStreamOpenMPT::_bind_methods() {
    ClassDB::bind_method(D_METHOD("load_from_file", "path"), &AudioStreamOpenMPT::load_from_file);
    ClassDB::bind_method(D_METHOD("load_from_data", "data"), &AudioStreamOpenMPT::load_from_data);

    ClassDB::bind_method(D_METHOD("set_data", "data"), &AudioStreamOpenMPT::set_data);
    ClassDB::bind_method(D_METHOD("get_data"), &AudioStreamOpenMPT::get_data);
    ClassDB::add_property("AudioStreamOpenMPT", PropertyInfo(Variant::PACKED_BYTE_ARRAY, "data"), "set_data", "get_data");

    ClassDB::bind_method(D_METHOD("set_mix_rate", "mix_rate"), &AudioStreamOpenMPT::set_mix_rate);
    ClassDB::bind_method(D_METHOD("get_mix_rate"), &AudioStreamOpenMPT::get_mix_rate);
    ClassDB::add_property("AudioStreamOpenMPT", PropertyInfo(Variant::INT, "mix_rate"), "set_mix_rate", "get_mix_rate");

    ClassDB::bind_method(D_METHOD("get_title"), &AudioStreamOpenMPT::get_title);
    ClassDB::bind_method(D_METHOD("get_artist"), &AudioStreamOpenMPT::get_artist);
    ClassDB::bind_method(D_METHOD("get_message"), &AudioStreamOpenMPT::get_message);
    ClassDB::bind_method(D_METHOD("get_num_channels"), &AudioStreamOpenMPT::get_num_channels);
    ClassDB::bind_method(D_METHOD("get_num_orders"), &AudioStreamOpenMPT::get_num_orders);
    ClassDB::bind_method(D_METHOD("get_num_patterns"), &AudioStreamOpenMPT::get_num_patterns);
    ClassDB::bind_method(D_METHOD("get_num_instruments"), &AudioStreamOpenMPT::get_num_instruments);
    ClassDB::bind_method(D_METHOD("get_num_samples"), &AudioStreamOpenMPT::get_num_samples);
    ClassDB::bind_method(D_METHOD("get_channel_names"), &AudioStreamOpenMPT::get_channel_names);
    ClassDB::bind_method(D_METHOD("get_instrument_names"), &AudioStreamOpenMPT::get_instrument_names);
    ClassDB::bind_method(D_METHOD("get_sample_names"), &AudioStreamOpenMPT::get_sample_names);
}

AudioStreamOpenMPT::AudioStreamOpenMPT() {
    length = 0.0;
    mix_rate = 48000;
}

AudioStreamOpenMPT::~AudioStreamOpenMPT() {
}

Ref<AudioStreamPlayback> AudioStreamOpenMPT::_instantiate_playback() const {
    Ref<AudioStreamPlaybackOpenMPT> playback;
    playback.instantiate();
    playback->set_stream(Ref<AudioStreamOpenMPT>(this));
    return playback;
}

String AudioStreamOpenMPT::_get_stream_name() const {
    if (data.size() == 0) {
        return "AudioStreamOpenMPT";
    }

    openmpt_module* mod = safe_create_module(data.ptr(), data.size());
    if (!mod) {
        return "AudioStreamOpenMPT";
    }

    const char* title = openmpt_module_get_metadata(mod, "title");
    String result = (title && strlen(title) > 0) ? String::utf8(title) : String("AudioStreamOpenMPT");
    openmpt_free_string(title);
    openmpt_module_destroy(mod);

    return result;
}

double AudioStreamOpenMPT::_get_length() const {
    if (data.size() == 0) {
        return 0.0;
    }

    openmpt_module* mod = safe_create_module(data.ptr(), data.size());
    if (!mod) {
        return 0.0;
    }

    double duration = openmpt_module_get_duration_seconds(mod);
    openmpt_module_destroy(mod);
    return duration;
}

bool AudioStreamOpenMPT::_is_monophonic() const {
    return false;
}

double AudioStreamOpenMPT::_get_bpm() const {
    if (data.size() == 0) {
        return 0.0;
    }

    openmpt_module* mod = safe_create_module(data.ptr(), data.size());
    if (!mod) {
        return 0.0;
    }

    double tempo = openmpt_module_get_current_tempo(mod);
    openmpt_module_destroy(mod);
    return tempo;
}

int AudioStreamOpenMPT::_get_beat_count() const {
    return 0;
}

Error AudioStreamOpenMPT::load_from_file(const String &p_path) {
    Ref<FileAccess> file = FileAccess::open(p_path, FileAccess::READ);
    if (file.is_null()) {
        UtilityFunctions::push_error("Failed to open file: ", p_path);
        return ERR_FILE_CANT_OPEN;
    }

    int64_t len = file->get_length();
    PackedByteArray file_data;
    file_data.resize(len);
    file->get_buffer(file_data.ptrw(), len);

    file_path = p_path;
    return load_from_data(file_data);
}

Error AudioStreamOpenMPT::load_from_data(const PackedByteArray &p_data) {
    if (p_data.size() == 0) {
        UtilityFunctions::push_error("Empty data provided");
        return ERR_INVALID_DATA;
    }

    openmpt_module* test_mod = safe_create_module(p_data.ptr(), p_data.size());
    if (!test_mod) {
        UtilityFunctions::push_error("Failed to load module: invalid data");
        return ERR_INVALID_DATA;
    }

    data = p_data;
    length = openmpt_module_get_duration_seconds(test_mod);
    openmpt_module_destroy(test_mod);
    return OK;
}

void AudioStreamOpenMPT::set_data(const PackedByteArray &p_data) {
    load_from_data(p_data);
}

PackedByteArray AudioStreamOpenMPT::get_data() const {
    return data;
}

void AudioStreamOpenMPT::set_mix_rate(int p_mix_rate) {
    mix_rate = p_mix_rate;
}

int AudioStreamOpenMPT::get_mix_rate() const {
    return mix_rate;
}

String AudioStreamOpenMPT::get_title() const {
    if (data.size() == 0) return "";

    openmpt_module* mod = safe_create_module(data.ptr(), data.size());
    if (!mod) return "";

    const char* title = openmpt_module_get_metadata(mod, "title");
    String result = title ? String::utf8(title) : String("");
    openmpt_free_string(title);
    openmpt_module_destroy(mod);
    return result;
}

String AudioStreamOpenMPT::get_artist() const {
    if (data.size() == 0) return "";

    openmpt_module* mod = safe_create_module(data.ptr(), data.size());
    if (!mod) return "";

    const char* artist = openmpt_module_get_metadata(mod, "artist");
    String result = artist ? String::utf8(artist) : String("");
    openmpt_free_string(artist);
    openmpt_module_destroy(mod);
    return result;
}

String AudioStreamOpenMPT::get_message() const {
    if (data.size() == 0) return "";

    openmpt_module* mod = safe_create_module(data.ptr(), data.size());
    if (!mod) return "";

    const char* message = openmpt_module_get_metadata(mod, "message");
    String result = message ? String::utf8(message) : String("");
    openmpt_free_string(message);
    openmpt_module_destroy(mod);
    return result;
}

int AudioStreamOpenMPT::get_num_channels() const {
    if (data.size() == 0) return 0;

    openmpt_module* mod = safe_create_module(data.ptr(), data.size());
    if (!mod) return 0;

    int num = openmpt_module_get_num_channels(mod);
    openmpt_module_destroy(mod);
    return num;
}

int AudioStreamOpenMPT::get_num_orders() const {
    if (data.size() == 0) return 0;

    openmpt_module* mod = safe_create_module(data.ptr(), data.size());
    if (!mod) return 0;

    int num = openmpt_module_get_num_orders(mod);
    openmpt_module_destroy(mod);
    return num;
}

int AudioStreamOpenMPT::get_num_patterns() const {
    if (data.size() == 0) return 0;

    openmpt_module* mod = safe_create_module(data.ptr(), data.size());
    if (!mod) return 0;

    int num = openmpt_module_get_num_patterns(mod);
    openmpt_module_destroy(mod);
    return num;
}

int AudioStreamOpenMPT::get_num_instruments() const {
    if (data.size() == 0) return 0;

    openmpt_module* mod = safe_create_module(data.ptr(), data.size());
    if (!mod) return 0;

    int num = openmpt_module_get_num_instruments(mod);
    openmpt_module_destroy(mod);
    return num;
}

int AudioStreamOpenMPT::get_num_samples() const {
    if (data.size() == 0) return 0;

    openmpt_module* mod = safe_create_module(data.ptr(), data.size());
    if (!mod) return 0;

    int num = openmpt_module_get_num_samples(mod);
    openmpt_module_destroy(mod);
    return num;
}

PackedStringArray AudioStreamOpenMPT::get_channel_names() const {
    PackedStringArray names;
    if (data.size() == 0) return names;

    openmpt_module* mod = safe_create_module(data.ptr(), data.size());
    if (!mod) return names;

    int num_channels = openmpt_module_get_num_channels(mod);
    for (int i = 0; i < num_channels; i++) {
        const char* name = openmpt_module_get_channel_name(mod, i);
        names.push_back(name ? String::utf8(name) : String(""));
        openmpt_free_string(name);
    }

    openmpt_module_destroy(mod);
    return names;
}

PackedStringArray AudioStreamOpenMPT::get_order_names() const {
    PackedStringArray names;
    if (data.size() == 0) return names;

    openmpt_module* mod = safe_create_module(data.ptr(), data.size());
    if (!mod) return names;

    int num_orders = openmpt_module_get_num_orders(mod);
    for (int i = 0; i < num_orders; i++) {
        const char* name = openmpt_module_get_order_name(mod, i);
        names.push_back(name ? String::utf8(name) : String(""));
        openmpt_free_string(name);
    }

    openmpt_module_destroy(mod);
    return names;
}

PackedStringArray AudioStreamOpenMPT::get_pattern_names() const {
    PackedStringArray names;
    if (data.size() == 0) return names;

    openmpt_module* mod = safe_create_module(data.ptr(), data.size());
    if (!mod) return names;

    int num_patterns = openmpt_module_get_num_patterns(mod);
    for (int i = 0; i < num_patterns; i++) {
        const char* name = openmpt_module_get_pattern_name(mod, i);
        names.push_back(name ? String::utf8(name) : String(""));
        openmpt_free_string(name);
    }

    openmpt_module_destroy(mod);
    return names;
}

PackedStringArray AudioStreamOpenMPT::get_instrument_names() const {
    PackedStringArray names;
    if (data.size() == 0) return names;

    openmpt_module* mod = safe_create_module(data.ptr(), data.size());
    if (!mod) return names;

    int num_instruments = openmpt_module_get_num_instruments(mod);
    for (int i = 0; i < num_instruments; i++) {
        const char* name = openmpt_module_get_instrument_name(mod, i);
        names.push_back(name ? String::utf8(name) : String(""));
        openmpt_free_string(name);
    }

    openmpt_module_destroy(mod);
    return names;
}

PackedStringArray AudioStreamOpenMPT::get_sample_names() const {
    PackedStringArray names;
    if (data.size() == 0) return names;

    openmpt_module* mod = safe_create_module(data.ptr(), data.size());
    if (!mod) return names;

    int num_samples = openmpt_module_get_num_samples(mod);
    for (int i = 0; i < num_samples; i++) {
        const char* name = openmpt_module_get_sample_name(mod, i);
        names.push_back(name ? String::utf8(name) : String(""));
        openmpt_free_string(name);
    }

    openmpt_module_destroy(mod);
    return names;
}

// AudioStreamPlaybackOpenMPT

void AudioStreamPlaybackOpenMPT::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_position", "position"), &AudioStreamPlaybackOpenMPT::set_position);
    ClassDB::bind_method(D_METHOD("get_position"), &AudioStreamPlaybackOpenMPT::get_position);

    ClassDB::bind_method(D_METHOD("set_repeat_count", "count"), &AudioStreamPlaybackOpenMPT::set_repeat_count);
    ClassDB::bind_method(D_METHOD("get_repeat_count"), &AudioStreamPlaybackOpenMPT::get_repeat_count);

    ClassDB::bind_method(D_METHOD("set_tempo_factor", "factor"), &AudioStreamPlaybackOpenMPT::set_tempo_factor);
    ClassDB::bind_method(D_METHOD("get_tempo_factor"), &AudioStreamPlaybackOpenMPT::get_tempo_factor);

    ClassDB::bind_method(D_METHOD("set_pitch_factor", "factor"), &AudioStreamPlaybackOpenMPT::set_pitch_factor);
    ClassDB::bind_method(D_METHOD("get_pitch_factor"), &AudioStreamPlaybackOpenMPT::get_pitch_factor);

    ClassDB::bind_method(D_METHOD("get_current_order"), &AudioStreamPlaybackOpenMPT::get_current_order);
    ClassDB::bind_method(D_METHOD("get_current_pattern"), &AudioStreamPlaybackOpenMPT::get_current_pattern);
    ClassDB::bind_method(D_METHOD("get_current_row"), &AudioStreamPlaybackOpenMPT::get_current_row);

    ClassDB::bind_method(D_METHOD("set_position_order_row", "order", "row"), &AudioStreamPlaybackOpenMPT::set_position_order_row);
}

AudioStreamPlaybackOpenMPT::AudioStreamPlaybackOpenMPT() {
    module = nullptr;
    position = 0.0;
    active = false;
    loops = 0;
}

AudioStreamPlaybackOpenMPT::~AudioStreamPlaybackOpenMPT() {
    if (module) {
        openmpt_module_destroy(module);
        module = nullptr;
    }
}

void AudioStreamPlaybackOpenMPT::_load_module() {
    if (stream.is_null() || stream->data.size() == 0) {
        return;
    }

    if (module) {
        openmpt_module_destroy(module);
    }

    module = safe_create_module(stream->data.ptr(), stream->data.size());
    if (!module) {
        UtilityFunctions::push_error("Failed to load module: invalid data");
        return;
    }

    openmpt_module_set_repeat_count(module, -1);
}

void AudioStreamPlaybackOpenMPT::set_stream(const Ref<AudioStreamOpenMPT> &p_stream) {
    stream = p_stream;
    _load_module();
}

void AudioStreamPlaybackOpenMPT::_start(double p_from_pos) {
    if (!module) {
        _load_module();
    }

    if (module) {
        openmpt_module_set_position_seconds(module, p_from_pos);
        position = p_from_pos;
        active = true;
        loops = 0;
    }
}

void AudioStreamPlaybackOpenMPT::_stop() {
    active = false;
}

bool AudioStreamPlaybackOpenMPT::_is_playing() const {
    return active;
}

int AudioStreamPlaybackOpenMPT::_get_loop_count() const {
    return loops;
}

double AudioStreamPlaybackOpenMPT::_get_playback_position() const {
    return position;
}

void AudioStreamPlaybackOpenMPT::_seek(double p_time) {
    if (module) {
        openmpt_module_set_position_seconds(module, p_time);
        position = p_time;
    }
}

int AudioStreamPlaybackOpenMPT::_mix(AudioFrame *p_buffer, double p_rate_scale, int p_frames) {
    if (!module || !active) {
        return 0;
    }

    std::vector<float> left(p_frames);
    std::vector<float> right(p_frames);

    size_t count = openmpt_module_read_float_stereo(module, stream->mix_rate, p_frames, left.data(), right.data());

    for (size_t i = 0; i < count; i++) {
        p_buffer[i].left = left[i];
        p_buffer[i].right = right[i];
    }

    position = openmpt_module_get_position_seconds(module);

    return count;
}

void AudioStreamPlaybackOpenMPT::_tag_used_streams() {
    if (stream.is_valid()) {
        stream->tag_used(0);
    }
}

void AudioStreamPlaybackOpenMPT::set_position(double p_position) {
    _seek(p_position);
}

double AudioStreamPlaybackOpenMPT::get_position() const {
    return position;
}

void AudioStreamPlaybackOpenMPT::set_repeat_count(int p_count) {
    if (module) {
        openmpt_module_set_repeat_count(module, p_count);
    }
}

int AudioStreamPlaybackOpenMPT::get_repeat_count() const {
    if (module) {
        return openmpt_module_get_repeat_count(module);
    }
    return 0;
}

void AudioStreamPlaybackOpenMPT::set_tempo_factor(double p_factor) {
    if (module) {
        openmpt_module_ctl_set_floatingpoint(module, "play.tempo_factor", p_factor);
    }
}

double AudioStreamPlaybackOpenMPT::get_tempo_factor() const {
    if (module) {
        return openmpt_module_ctl_get_floatingpoint(module, "play.tempo_factor");
    }
    return 1.0;
}

void AudioStreamPlaybackOpenMPT::set_pitch_factor(double p_factor) {
    if (module) {
        openmpt_module_ctl_set_floatingpoint(module, "play.pitch_factor", p_factor);
    }
}

double AudioStreamPlaybackOpenMPT::get_pitch_factor() const {
    if (module) {
        return openmpt_module_ctl_get_floatingpoint(module, "play.pitch_factor");
    }
    return 1.0;
}

int AudioStreamPlaybackOpenMPT::get_current_order() const {
    if (module) {
        return openmpt_module_get_current_order(module);
    }
    return 0;
}

int AudioStreamPlaybackOpenMPT::get_current_pattern() const {
    if (module) {
        return openmpt_module_get_current_pattern(module);
    }
    return 0;
}

int AudioStreamPlaybackOpenMPT::get_current_row() const {
    if (module) {
        return openmpt_module_get_current_row(module);
    }
    return 0;
}

void AudioStreamPlaybackOpenMPT::set_position_order_row(int p_order, int p_row) {
    if (module) {
        openmpt_module_set_position_order_row(module, p_order, p_row);
        position = openmpt_module_get_position_seconds(module);
    }
}
