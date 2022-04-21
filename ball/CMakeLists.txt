find_package(Qt5 COMPONENTS Gui REQUIRED)

add_subdirectory(tests)

add_library(ball SHARED
    ball.cpp
    real-ball.cpp
)
target_include_directories(ball PRIVATE .)
target_include_directories(ball PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(ball PRIVATE Qt::Gui)
