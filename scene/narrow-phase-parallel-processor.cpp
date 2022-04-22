#include "narrow-phase-parallel-processor.h"
#include <optional>
#include <chrono>

namespace Balls {


void NarrowPhaseParallelProcessor::start() {
    unsigned int threads_count = std::thread::hardware_concurrency() - 1;  // one core for scene thread
    ball_workers.reserve(threads_count);
    auto self = shared_from_this();
    for (std::size_t i = 0; i < threads_count; ++i) {
        ball_workers.emplace_back([self] () {
            self->work();
        });
    }
}

void NarrowPhaseParallelProcessor::stop() {
    auto self = shared_from_this();
    {
        std::unique_lock<std::mutex> guard(tasks_mutex);
        if (shutdown) {
            return;
        }
        shutdown = true;
    }
    tasks_cv.notify_all();
    for (auto& w : ball_workers) {
        w.join();
    }
    ball_workers.clear();
}

void NarrowPhaseParallelProcessor::process_potential_collisions(UniqueBallPairs potential_collisions) {
    using namespace std::chrono_literals;

    tasks_left = potential_collisions.size();
    std::swap(tasks, potential_collisions);
    task_it = tasks.begin();
    tasks_cv.notify_all();

    auto t1 = std::chrono::high_resolution_clock::now();

    auto self = shared_from_this();
    std::unique_lock<std::mutex> guard(tasks_counter_mutex);
    tasks_counter_cv.wait(guard, [self] () {
        return self->tasks_left == 0;
    });
    auto t2 = std::chrono::high_resolution_clock::now();
    std::swap(colliding_pairs, new_colliding_pairs);
    new_colliding_pairs.clear();
    tasks = UniqueBallPairs();
}

void NarrowPhaseParallelProcessor::work() {
    auto self = shared_from_this();
    while (true) {
        std::optional<BallPair> bp;
        {
            std::unique_lock<std::mutex> guard(tasks_mutex);
            tasks_cv.wait(guard, [self] () {
                return self->shutdown ||
                    (!self->tasks.empty() && self->task_it != self->tasks.end());
            });
            if (shutdown) {
                break;
            }
            bp = *task_it;
            ++task_it;
        }
        if (bp) {
            const BallPair& bpv = bp.value();
            process_task(bpv);
            bool last = false;
            {
                std::lock_guard<std::mutex> guard(tasks_counter_mutex);
                last = (--tasks_left == 0);
            }
            if (last) {
                tasks_counter_cv.notify_one();
            }
        }
    }
}

void NarrowPhaseParallelProcessor::process_task(const BallPair& bp) {
    const std::shared_ptr<Ball>& bb1 = bp.first;
    const std::shared_ptr<Ball>& bb2 = bp.second;

    bool is_colliding = bb1->collides(bb2);
    if (is_colliding) {
        bool was_colliding = false;
        {
            std::lock_guard<std::mutex> guard(colliding_mutex);
            was_colliding = colliding_pairs.find(bp) != colliding_pairs.end();
            new_colliding_pairs.insert(bp);
        }
        if (!was_colliding) {
            bb1->process_collision(bb2);
        }
    }
}


}
