add_library(
    "exp-common"
    INTERFACE
    tokens.hpp
)

target_include_directories("exp-common" INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})