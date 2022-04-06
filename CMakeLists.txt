cmake_minimum_required(VERSION 3.0)

project(balls LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(balls
    main.cpp
    mainwidget.cpp
    mainwidget.h
    ballswidget.cpp
    ballswidget.h
    ball/ball.cpp
    ball/ball.h
    ball/real-ball.cpp
    ball/real-ball.h
    scene/scene-manager.h
    scene/scene-manager.cpp
    scene/common-scene-manager.h
    scene/common-scene-manager.cpp
    scene/quattro-check-scene-manager.h
    scene/quattro-check-scene-manager.cpp
    scene/sweep-and-prune-scene-manager.h
    scene/sweep-and-prune-scene-manager.cpp
)

target_link_libraries(balls PRIVATE Qt5::Widgets)
