#include "test_interface.h"

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest/doctest.h"

void TestRunner::_bind_methods() {
    ClassDB::bind_method(D_METHOD("run_tests"), &TestRunner::run_tests);
}

TestRunner::TestRunner() {
}

TestRunner::~TestRunner() {
}

int TestRunner::run_tests() {
    // Run doctest with Godot runtime initialized
    doctest::Context context;

    // Configure doctest
    const char* argv[] = {"tests"};
    context.applyCommandLine(1, argv);

    // Run tests and return result
    return context.run();
}
