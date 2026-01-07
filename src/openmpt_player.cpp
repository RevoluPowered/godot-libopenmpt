#include "openmpt_player.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void OpenMPTPlayer::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_amplitude"), &OpenMPTPlayer::get_amplitude);
    ClassDB::bind_method(D_METHOD("set_amplitude", "p_amplitude"), &OpenMPTPlayer::set_amplitude);
    ClassDB::add_property("OpenMPTPlayer", PropertyInfo(Variant::FLOAT, "amplitude"), "set_amplitude", "get_amplitude");
}

OpenMPTPlayer::OpenMPTPlayer() {
    time_passed = 0.0;
}

OpenMPTPlayer::~OpenMPTPlayer() {
}

void OpenMPTPlayer::_process(double delta) {
    time_passed += delta;
}

void OpenMPTPlayer::set_amplitude(const double p_amplitude) {
    UtilityFunctions::print("OpenMPTPlayer: Set amplitude to ", p_amplitude);
}

double OpenMPTPlayer::get_amplitude() const {
    return 1.0;
}
