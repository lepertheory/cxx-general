add_library(Arb OBJECT Arb.c++)
target_include_directories(Arb PUBLIC .. .)
target_link_libraries(Arb PUBLIC ArbInt)
