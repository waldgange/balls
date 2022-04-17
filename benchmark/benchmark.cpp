#include "benchmark.h"

#include <chrono>
#include <unordered_map>

namespace Benchmark {


BenchmarkArguments parse_command_line_arguments(int argc, char* argv[]) {
    if (argc != 5) {
        throw std::runtime_error(
            "This programm gets exactly 4 arguments. Example:\n"
            "benchmark <broad_phase_type> <narrow_phase_type> <number_of_balls> <number_of_frames>");
    }
    std::string broad = argv[1];
    std::string narrow = argv[2];
    const int balls = std::atoi(argv[3]);
    const int frames = std::atoi(argv[4]);

    static const std::unordered_map<std::string, Balls::BroadPhaseType> broad_types = {
        {"qw", Balls::BroadPhaseType::QWATTRO},
        {"ss", Balls::BroadPhaseType::SORT_AND_SWEEP}
    };
    static const std::unordered_map<std::string, Balls::NarrowPhaseType> narrow_types = {
        {"s",  Balls::NarrowPhaseType::SEQUENT},
        {"p",  Balls::NarrowPhaseType::PARALLEL}
    };
    auto it1 = broad_types.find(broad);
    if (it1 == broad_types.end()) {
        throw std::runtime_error("Invalid broad phase type passed in first argument");
    }
    Balls::BroadPhaseType bt = it1->second;
    auto it2 = narrow_types.find(narrow);
    if (it2 == narrow_types.end()) {
        throw std::runtime_error("Invalid narrow phase type passed in second argument");
    }
    Balls::NarrowPhaseType nt = it2->second;
    if (balls < 1) {
        throw std::runtime_error("Invalid balls count in second argument");
    }
    if (frames < 1) {
        throw std::runtime_error("Invalid frames count in third argument");
    }

    BenchmarkArguments result = {bt, nt, balls, frames};

    return result;
}

BenchmarkManager::BenchmarkManager(const BenchmarkArguments& _ba)
    : ba(_ba)
    , sm(Balls::make_scene_manager(_ba.broad, _ba.narrow)) {}

void BenchmarkManager::prepare() {
    sm->set_size(10000, 10000);
    float delta = 10000.0f / ba.balls;
    for (int i = 0; i < ba.balls; ++i) {
        sm->add_ball(Balls::make_ball(Balls::BallType::REAL_BALL, delta * i, delta * i));
    }
    sm->start();
}

double BenchmarkManager::run() {
    using namespace std::chrono_literals;
    auto t_start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < ba.frames; ++i) {
        sm->get_next_frame();
    }

    auto t_end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(t_end-t_start).count();
}



}
