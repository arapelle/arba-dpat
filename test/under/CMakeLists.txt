
find_package(GTest CONFIG REQUIRED)

add_cpp_library_basic_tests(${PROJECT_NAME} GTest::gtest_main
    SOURCES
        under_tests.cpp
        under_substiuable_tests.cpp
        under_optional_tests.cpp
)
