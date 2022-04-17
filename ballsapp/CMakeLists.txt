find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(ballsapp
    main.cpp
    mainwidget.cpp
    ballswidget.cpp
)

target_link_libraries(ballsapp PRIVATE Qt::Widgets)
target_link_libraries(ballsapp LINK_PUBLIC scene)
