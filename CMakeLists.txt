cmake_minimum_required(VERSION 3.22)

file(READ config/VER_MAJOR ver_major)
file(READ config/VER_MINOR ver_minor)
file(READ config/VER_PATCH ver_patch)

project(cxx-general VERSION "${ver_major}.${ver_minor}.${ver_patch}")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(ArbInt)

add_library(cxx-general SHARED $<TARGET_OBJECTS:ArbInt>)

enable_testing()
add_subdirectory(Tests)
