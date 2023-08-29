add_library(
    "common_compiler_options"
    INTERFACE
)

target_compile_options("common_compiler_options" INTERFACE 
    $<$<CXX_COMPILER_ID:GNU>:$<$<CONFIG:DEBUG>:-fsanitize=address -fsanitize=undefined -pedantic -Wall -Wextra>>
    $<$<CXX_COMPILER_ID:Clang>:$<$<CONFIG:DEBUG>:-fsanitize=address -fsanitize=undefined -pedantic -Wall -Wextra>>
)

target_link_options("common_compiler_options" INTERFACE 
    $<$<CXX_COMPILER_ID:GNU>:$<$<CONFIG:DEBUG>:-fsanitize=address -fsanitize=undefined>>
    $<$<CXX_COMPILER_ID:Clang>:$<$<CONFIG:DEBUG>:-fsanitize=address -fsanitize=undefined>>
)

add_executable("expression-parser" main.cpp Lexer.cpp Errors.cpp Parser.cpp)
target_link_libraries("expression-parser" PRIVATE common_compiler_options)


