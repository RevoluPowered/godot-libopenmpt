#ifndef TEST_INTERFACE_H
#define TEST_INTERFACE_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Interface to run doctest tests from within Godot runtime
class TestRunner : public RefCounted {
    GDCLASS(TestRunner, RefCounted)

protected:
    static void _bind_methods();

public:
    TestRunner();
    ~TestRunner();

    // Run all tests and return exit code (0 = success)
    int run_tests();
};

#endif // TEST_INTERFACE_H
