find_package(Qt5 COMPONENTS Core REQUIRED)

add_subdirectory(tests)

add_library(scene SHARED
    scene-manager.cpp
    real-scene-manager.cpp
    pre-phase-sequent-processor.cpp
    pre-phase-parallel-processor.cpp
    broad-phase-quattro-detector.cpp
    broad-phase-sort-and-sweep-detector.cpp
    narrow-phase-sequent-processor.cpp
    narrow-phase-parallel-processor.cpp
)

target_include_directories(scene PRIVATE .)
target_include_directories(scene PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(scene PRIVATE Qt::Core)
target_link_libraries(scene LINK_PUBLIC ball)
