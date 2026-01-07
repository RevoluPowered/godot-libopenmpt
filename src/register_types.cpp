#include "register_types.h"
#include "audio_stream_openmpt.h"
#include "resource_loader_openmpt.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

static Ref<ResourceLoaderOpenMPT> resource_loader_openmpt;

void initialize_libopenmpt_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }

    ClassDB::register_class<AudioStreamOpenMPT>();
    ClassDB::register_class<AudioStreamPlaybackOpenMPT>();
    ClassDB::register_class<ResourceLoaderOpenMPT>();

    resource_loader_openmpt.instantiate();
    ResourceLoader::get_singleton()->add_resource_format_loader(resource_loader_openmpt);
}

void uninitialize_libopenmpt_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }

    ResourceLoader::get_singleton()->remove_resource_format_loader(resource_loader_openmpt);
    resource_loader_openmpt.unref();
}

extern "C" {
    GDExtensionBool GDE_EXPORT libopenmpt_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
        godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

        init_obj.register_initializer(initialize_libopenmpt_module);
        init_obj.register_terminator(uninitialize_libopenmpt_module);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

        return init_obj.init();
    }
}
