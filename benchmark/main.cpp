#include "benchmark.h"

#include <optional>
#include <iostream>


int main(int argc, char *argv[])
{
    std::optional<Benchmark::BenchmarkArguments> ba;
    try {
        ba.emplace(Benchmark::parse_command_line_arguments(argc, argv));
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    if (!ba) {
        return 1;
    }
    Benchmark::BenchmarkManager bm(ba.value());
    std::cout << "adding " << ba->balls << " balls..." << std::endl;
    bm.prepare();
    std::cout << "processing " << ba->frames << " frames..." << std::endl;
    double sec = bm.run();
    std::cout << ba->frames << " frames processed in " << sec << " seconds" << std::endl;
    return 0;
}
