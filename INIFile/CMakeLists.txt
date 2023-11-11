add_library(INIFile OBJECT INIFile.c++)
target_include_directories(INIFile PUBLIC ..)
target_link_libraries(INIFile PUBLIC ValReader tokenize trim)
