cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

project(pygerber_gerber_parser_cpp_project LANGUAGES CXX VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/CPM.cmake)

CPMAddPackage(
    NAME fmt
    VERSION 8.0.1
    URL https://github.com/fmtlib/fmt/archive/refs/tags/11.0.2.zip
)


add_library(GerberParserCppModule STATIC)
target_sources(
    GerberParserCppModule PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
    cpp/src/GerberParserCppModule.cxx
)
target_link_libraries(
    GerberParserCppModule
    PRIVATE fmt::fmt
)
target_compile_features(GerberParserCppModule PUBLIC cxx_std_20)

# Download and load pybind11 with CMake Package Manager
# See https://github.com/cpm-cmake/CPM.cmake/blob/master/README.md
CPMAddPackage(
    NAME pybind11
    VERSION 2.13.6
    URL https://github.com/pybind/pybind11/archive/refs/tags/v2.13.6.zip
)

pybind11_add_module(PyGerberGerberParserCpp cpp/src/PythonModule.cpp)
target_link_libraries(
    PyGerberGerberParserCpp
    PRIVATE fmt::fmt
    PRIVATE GerberParserCppModule
)
target_compile_features(PyGerberGerberParserCpp PRIVATE cxx_std_20)

add_custom_command(
    TARGET PyGerberGerberParserCpp
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:PyGerberGerberParserCpp>
        ${PROJECT_SOURCE_DIR}/python/pygerber_gerber_parser_cpp/gerber_parser$<TARGET_FILE_SUFFIX:PyGerberGerberParserCpp>
)

CPMAddPackage(
    NAME Catch2
    VERSION 3.3.2
    URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.3.2.zip
)


file(GLOB_RECURSE test_source_files ${PROJECT_SOURCE_DIR}/cpp/test/ *.test.cpp)
add_executable(tests)
target_sources(tests PUBLIC ${test_source_files})

target_link_libraries(
    tests
    PRIVATE fmt::fmt
    PRIVATE Catch2::Catch2WithMain
    PRIVATE GerberParserCppModule
)
target_compile_features(tests PRIVATE cxx_std_20)
