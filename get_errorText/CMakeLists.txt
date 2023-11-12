include(CheckSymbolExists)

add_library(get_errorText OBJECT get_errorText.c++)
target_include_directories(get_errorText PUBLIC .. .)

check_symbol_exists(strerror_r string.h HasErrortextReentrant)
if (HasErrortextReentrant)
  # I don't think this does anything. Double-check.
  target_compile_definitions(get_errorText PUBLIC GET_ERRORTEXT_REENTRANT)
endif ()
