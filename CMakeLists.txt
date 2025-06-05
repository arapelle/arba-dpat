cmake_minimum_required(VERSION 3.26)

# EXTERNAL DEPENDENCIES

add_subdirectory(external)

# CMAKE INCLUDES

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/external/cmake/)
include(CMakePrintHelpers)
include(cmtk/CppLibraryProject)

# CONFIGURATION

disable_in_source_build()
set_build_type_ifndef()

# C++ PROJECT

set_project_name(NAMESPACE "arba" BASE_NAME "dpat")
string(TIMESTAMP configure_datetime "%Y%m%d-%H%M%S")
set_project_semantic_version("0.1.0" PRE_RELEASE "dev" BUILD_METADATA "${configure_datetime}")

project(${PROJECT_NAME}
        VERSION ${PROJECT_VERSION}
        DESCRIPTION "A C++ library."
        HOMEPAGE_URL "https://github.com/arapelle/arba-dpat"
        LANGUAGES CXX)
message(STATUS "PROJECT : ${PROJECT_NAME} ${PROJECT_SEMANTIC_VERSION} ${CMAKE_BUILD_TYPE}")

# C++ LIBRARY

## Generated/Configured headers:
configure_headers(configured_headers
    FILES
        include/${PROJECT_NAMESPACE}/${PROJECT_FEATURE_NAME}/version.hpp.in
)

## Headers:
set(headers
    include/arba/dpat/singleton/singleton.hpp
    include/arba/dpat/state/state.hpp
    include/arba/dpat/state/state_machine.hpp
    include/arba/dpat/state/basic_state.hpp
    include/arba/dpat/state/basic_state_machine.hpp
)

## Add C++ library:
cxx_standard_option(${PROJECT_UPPER_VAR_NAME}_CXX_STANDARD MIN 20 MAX 26)

add_cpp_library(${PROJECT_NAME} HEADER_ONLY
  HEADERS ${headers} ${configured_headers}
  CXX_STANDARD ${${PROJECT_UPPER_VAR_NAME}_CXX_STANDARD}
  DEFAULT_WARNING_OPTIONS
  DEFAULT_ERROR_OPTIONS
)
add_library("${PROJECT_NAMESPACE}::${PROJECT_FEATURE_NAME}${LIBRARY_TYPE_POSTFIX}" ALIAS ${PROJECT_NAME})

## Link C++ targets:
find_package(arba-itru 0.5.0 REQUIRED CONFIG)
target_link_libraries(${PROJECT_NAME}
    INTERFACE
        arba::itru
)

## Add tests:
add_test_subdirectory_if_build(test)

## Add examples:
add_example_subdirectory_if_build(example)

# C++ INSTALL

## Install C++ library:
install_cpp_libraries(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-targets)

## Install project package
install_library_package(${PROJECT_NAME} INPUT_PACKAGE_CONFIG_FILE cmake/config/package-config.cmake.in)
install_uninstall_script(${PROJECT_NAME})
