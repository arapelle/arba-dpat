
find_package(GTest CONFIG REQUIRED)

add_cpp_library_basic_tests(${PROJECT_NAME} GTest::gtest_main
    SOURCES
        singleton_tests.cpp
)
