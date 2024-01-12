cmake_minimum_required(VERSION 3.5)

project(SnakeGame VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
# open CMAKE_AUTOMOC and CMAKE_AUTOUIC
qt_standard_project_setup()

qt_add_executable(${PROJECT_NAME}
    main.cpp
    GameShowWidget.h
    GameShowWidget.cpp
    SnakeGameWidget.h
    SnakeGameWidget.cpp
)

# add img
# qt_add_resources(${PROJECT_NAME} "myres"
#     PREFIX "/"
#     FILES res/img/icon.ico
# )

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
