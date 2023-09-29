add_library(
    "expression-parser"
    STATIC
    Lexer.cpp Parser.cpp Errors.cpp
)

target_link_libraries("expression-parser" PUBLIC common_compiler_options "exp-common")
target_include_directories("expression-parser" PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
