find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(ballsapp
    main.cpp
    mainwidget.cpp
    ballswidget.cpp
)

target_link_libraries(ballsapp PRIVATE Qt::Widgets)
target_link_libraries(ballsapp LINK_PUBLIC scene)
