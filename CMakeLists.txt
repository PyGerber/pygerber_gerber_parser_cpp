cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

project(pygerber_gerber_parser_cpp_project LANGUAGES CXX VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

MESSAGE(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
MESSAGE(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")

include(cmake/CPM.cmake)

CPMAddPackage(
    NAME fmt
    VERSION 11.0.2
    URL https://github.com/fmtlib/fmt/archive/refs/tags/11.0.2.zip
    OPTIONS "CMAKE_POSITION_INDEPENDENT_CODE ON"
)

IF(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto=auto")
ENDIF()

ADD_LIBRARY(GerberParserCpp STATIC)
TARGET_COMPILE_FEATURES(GerberParserCpp PRIVATE cxx_std_20)
SET_TARGET_PROPERTIES(GerberParserCpp
    PROPERTIES POSITION_INDEPENDENT_CODE ON
)

FILE(GLOB_RECURSE gerber_library_source ${PROJECT_SOURCE_DIR}/cpp/gerber/src/*.cpp)
TARGET_SOURCES(GerberParserCpp PRIVATE ${gerber_library_source})

MESSAGE(STATUS "gerber_library_source:")
FOREACH(item IN LISTS gerber_library_source)
    MESSAGE(STATUS " - ${item}")
ENDFOREACH()


TARGET_INCLUDE_DIRECTORIES(GerberParserCpp PUBLIC cpp/gerber/include)
TARGET_LINK_LIBRARIES(
    GerberParserCpp
PRIVATE
    PUBLIC fmt::fmt
)

# Download and load pybind11 with CMake Package Manager
# See https://github.com/cpm-cmake/CPM.cmake/blob/master/README.md
CPMAddPackage(
    NAME pybind11
    VERSION 2.13.6
    URL https://github.com/pybind/pybind11/archive/refs/tags/v2.13.6.zip
)

pybind11_add_module(PyGerberGerberParserCpp cpp/python/gerber_parser.cpp)
target_compile_features(PyGerberGerberParserCpp PRIVATE cxx_std_20)
target_link_libraries(
    PyGerberGerberParserCpp
PRIVATE
    fmt::fmt
    GerberParserCpp
)

add_custom_command(
    TARGET PyGerberGerberParserCpp
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:PyGerberGerberParserCpp>
        ${PROJECT_SOURCE_DIR}/python/pygerber_gerber_parser_cpp/gerber_parser$<TARGET_FILE_SUFFIX:PyGerberGerberParserCpp>
)

CPMAddPackage("gh:catchorg/Catch2@3.3.2")


add_executable(tests)
target_compile_features(tests PRIVATE cxx_std_20)

file(GLOB_RECURSE test_source_files ${PROJECT_SOURCE_DIR}/cpp/test/ *.test.cpp)
target_sources(tests PRIVATE ${test_source_files})

MESSAGE(STATUS "test_source_files:")
FOREACH(item IN LISTS test_source_files)
    MESSAGE(STATUS " - ${item}")
ENDFOREACH()

target_link_libraries(
    tests
PRIVATE
    fmt::fmt
    Catch2::Catch2WithMain
    GerberParserCpp
)

include(CTest)
list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(tests)
