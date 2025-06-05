include(cmtk/CppLibraryTests)
include(GoogleTest)

find_package(GTest CONFIG REQUIRED)

add_cpp_library_basic_tests(${PROJECT_NAME} GTest::gtest_main
    SOURCES
        project_version_tests.cpp
        basic_state_tests.cpp
        basic_state_machine_tests.cpp
        state_machine_tests.cpp
)
