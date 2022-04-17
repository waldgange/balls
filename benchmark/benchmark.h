#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "scene-manager.h"

namespace Benchmark {

struct BenchmarkArguments {
    const Balls::BroadPhaseType broad;
    const Balls::NarrowPhaseType narrow;
    const int balls;
    const int frames;
};

BenchmarkArguments parse_command_line_arguments(int argc, char* argv[]);

class BenchmarkManager
{
    const BenchmarkArguments ba;
    std::shared_ptr<Balls::SceneManager> sm;
public:
    explicit BenchmarkManager(const BenchmarkArguments& _ba);
    void prepare();
    double run();
};


}

#endif // BENCHMARK_H
