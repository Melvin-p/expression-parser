add_library(
    "expression-core"
    STATIC
    Lexer.cpp Parser.cpp Errors.cpp ExpGen.cpp Random.cpp AST.cpp Node.cpp tokens.cpp Interpreter.cpp ActionTokens.cpp
)

target_link_libraries("expression-core" PRIVATE common_compiler_options)
target_include_directories("expression-core" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/public" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/private")
