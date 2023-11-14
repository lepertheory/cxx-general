include(FetchContent)

FetchContent_Declare(ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
    GIT_TAG v5.0.0
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
    FetchContent_Populate(ftxui)
    add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

#-- ┌─ FTXUI options ─────────────────────
#-- │ FTXUI_ENABLE_INSTALL     : ON
#-- │ FTXUI_BUILD_EXAMPLES     : OFF
#-- │ FTXUI_QUIET              : OFF
#-- │ FTXUI_BUILD_DOCS         : OFF
#-- │ FTXUI_BUILD_TESTS        : OFF
#-- │ FTXUI_BUILD_TESTS_FUZZER : OFF
#-- │ FTXUI_ENABLE_COVERAGE    : OFF
#-- │ FTXUI_DEV_WARNINGS       : OFF
#-- │ FTXUI_CLANG_TIDY         : OFF
#-- └─────────────────────────────────────

add_executable(arbCalc arbCalc.c++)
add_executable(longDiv longDiv.c++)
include_directories(..)
target_link_libraries(arbCalc
    PRIVATE cxx-general
    PRIVATE ftxui::component
    PRIVATE ftxui::dom
    PRIVATE ftxui::screen
)
