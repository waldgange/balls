cmake_minimum_required(VERSION 3.0)

project(balls LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_subdirectory (ball)
add_subdirectory (scene)

add_executable(balls
    main.cpp
    mainwidget.cpp
    ballswidget.cpp
)

target_link_libraries(balls PRIVATE Qt::Widgets scene)
