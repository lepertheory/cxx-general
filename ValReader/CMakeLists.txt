add_library(ValReader OBJECT ValReader.c++)
target_include_directories(ValReader PUBLIC .. .)
target_link_libraries(ValReader PUBLIC Arb)
