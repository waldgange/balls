set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(benchmark
    main.cpp
    benchmark.cpp
)

target_link_libraries(benchmark LINK_PUBLIC scene)
