find_package(Qt5 COMPONENTS Gui REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(ball SHARED
    ball.cpp
    real-ball.cpp
)
target_include_directories(ball PRIVATE .)
target_include_directories(ball PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(ball PRIVATE Qt::Gui)
