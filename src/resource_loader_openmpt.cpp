#include "resource_loader_openmpt.h"
#include "audio_stream_openmpt.h"

#include <godot_cpp/classes/file_access.hpp>

using namespace godot;

void ResourceLoaderOpenMPT::_bind_methods() {
}

PackedStringArray ResourceLoaderOpenMPT::_get_recognized_extensions() const {
    PackedStringArray extensions;
    extensions.push_back("mod");
    extensions.push_back("s3m");
    extensions.push_back("xm");
    extensions.push_back("it");
    extensions.push_back("mptm");
    extensions.push_back("stm");
    extensions.push_back("669");
    extensions.push_back("ptm");
    extensions.push_back("mtm");
    extensions.push_back("med");
    extensions.push_back("okt");
    extensions.push_back("far");
    extensions.push_back("ult");
    extensions.push_back("amf");
    return extensions;
}

bool ResourceLoaderOpenMPT::_handles_type(const StringName &p_type) const {
    return p_type == StringName("AudioStream") || p_type == StringName("AudioStreamOpenMPT");
}

String ResourceLoaderOpenMPT::_get_resource_type(const String &p_path) const {
    String extension = p_path.get_extension().to_lower();
    PackedStringArray recognized = _get_recognized_extensions();

    for (int i = 0; i < recognized.size(); i++) {
        if (extension == recognized[i]) {
            return "AudioStreamOpenMPT";
        }
    }

    return "";
}

Variant ResourceLoaderOpenMPT::_load(const String &p_path, const String &p_original_path, bool p_use_sub_threads, int32_t p_cache_mode) const {
    Ref<AudioStreamOpenMPT> stream;
    stream.instantiate();

    Error err = stream->load_from_file(p_path);
    if (err != OK) {
        return Variant();
    }

    return stream;
}
