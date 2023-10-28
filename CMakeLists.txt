cmake_minimum_required(VERSION 3.22)
project("expression-parser" 
    VERSION 0.1.0
    DESCRIPTION "A math expression parser"
    LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 20)
set(CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(BUILD_SHARED_LIBS ON)
option(lto "build with link time optimisation" ON)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(WARNING "Windows")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message(WARNING "MSVC")
endif()

if(lto)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT ipo_support OUTPUT output)
    if(ipo_support)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
        message(VERBOSE "Interprocedural optimization enabled")
    endif()
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/output")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/output")

add_subdirectory(./src/)

set(CMAKE_SKIP_INSTALL_RULES True)