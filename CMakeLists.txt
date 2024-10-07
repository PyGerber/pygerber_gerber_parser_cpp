cmake_minimum_required(VERSION 3.14 FATAL_ERROR)


project(
    pygerber_gerber_parser_cpp_project
    LANGUAGES CXX
    VERSION 0.0.1
)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/CPM.cmake)

# Download and load pybind11 with CMake Package Manager
# See https://github.com/cpm-cmake/CPM.cmake/blob/master/README.md
CPMAddPackage(
    NAME pybind11
    VERSION 2.13.6
    URL https://github.com/pybind/pybind11/archive/refs/tags/v2.13.6.zip
)

pybind11_add_module(pygerber_gerber_parser_cpp cpp/src/main.cpp)

add_custom_command(
    TARGET pygerber_gerber_parser_cpp
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:pygerber_gerber_parser_cpp>
        ${CMAKE_SOURCE_DIR}/python/pygerber_gerber_parser_cpp/$<TARGET_FILE_NAME:pygerber_gerber_parser_cpp>
)
