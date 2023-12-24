find_package(Qt6 COMPONENTS Widgets)

set(calculator_sources
        main.cpp
        mainwindow.h
        mainwindow.cpp
        mainwindow.ui
)

if(${Qt6_FOUND})
    message(DEBUG "qt6 found")
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)

    add_executable(calculator ${calculator_sources})
    target_link_libraries(calculator PRIVATE Qt6::Widgets "expression-core" "common_compiler_options")
    set_target_properties(calculator PROPERTIES WIN32_EXECUTABLE TRUE)
    
endif()
