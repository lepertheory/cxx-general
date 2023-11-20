add_executable(arbCalc arbCalc.c++)
add_executable(longDiv longDiv.c++)
include_directories(..)

target_link_libraries(arbCalc
    PRIVATE cxx-general
    PRIVATE notcurses
    PRIVATE notcurses-core
)
# FIXME: Use PkgConfig.
execute_process(COMMAND brew --prefix OUTPUT_VARIABLE homebrew_prefix OUTPUT_STRIP_TRAILING_WHITESPACE)
target_include_directories(arbCalc PUBLIC ${homebrew_prefix}/include)
target_link_directories(arbCalc PUBLIC ${homebrew_prefix}/lib)