add_executable(benchmark
    main.cpp
    benchmark.cpp
)

target_link_libraries(benchmark LINK_PUBLIC scene)
