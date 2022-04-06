cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_library (scene SHARED
    scene-manager.cpp
    common-scene-manager.cpp
    quattro-check-scene-manager.cpp
    sweep-and-prune-scene-manager.cpp
)

target_include_directories(scene PRIVATE .)
target_include_directories (scene PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries (scene LINK_PUBLIC ball)

