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

add_subdirectory(./expression_parser/)

add_executable("main" main.cpp)
target_link_libraries("main" PRIVATE "expression-parser")

if(DEFINED VCPKG_TOOLCHAIN)
    list(APPEND CMAKE_PREFIX_PATH "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/share/doctest")
endif()

find_package(doctest CONFIG)

if(${doctest_FOUND})
    add_executable("tests" tests.cpp)
    target_link_libraries("tests" PRIVATE doctest::doctest "expression-parser")
    add_test(NAME "tests" COMMAND "tests")
endif()
