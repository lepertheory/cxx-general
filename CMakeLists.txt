cmake_minimum_required(VERSION 3.22)

file(READ config/VER_MAJOR ver_major)
file(READ config/VER_MINOR ver_minor)
file(READ config/VER_PATCH ver_patch)

project(cxx-general VERSION "${ver_major}.${ver_minor}.${ver_patch}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(ArbInt)
add_subdirectory(Arb)
add_subdirectory(INIFile)
add_subdirectory(Timestamp)
add_subdirectory(ValReader)
add_subdirectory(get_errorText)
add_subdirectory(tokenize)
add_subdirectory(trim)
add_subdirectory(utilities)
add_subdirectory(wrapText)

add_library(cxx-general SHARED $<TARGET_OBJECTS:Arb>
                               $<TARGET_OBJECTS:ArbInt>
                               $<TARGET_OBJECTS:INIFile>
                               $<TARGET_OBJECTS:Timestamp>
                               $<TARGET_OBJECTS:ValReader>
                               $<TARGET_OBJECTS:get_errorText>
                               $<TARGET_OBJECTS:tokenize>
                               $<TARGET_OBJECTS:trim>
                               $<TARGET_OBJECTS:wrapText>)

enable_testing()
add_subdirectory(Tests)
