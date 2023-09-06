add_library(
    "expression-parser"
    OBJECT
    Lexer.cpp Errors.cpp Parser.cpp
)

target_link_libraries("expression-parser" PUBLIC common_compiler_options)
target_include_directories("expression-parser" PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
