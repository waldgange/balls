#include "narrow-phase-parallel-processor.h"
#include <optional>
#include <iostream>

namespace Balls {


NarrowPhaseParallelProcessor::~NarrowPhaseParallelProcessor() {
    stop();
}

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
        tasks_cv.wait(guard, [self] () {
            return self->tasks.empty();
        });
    }
    tasks_cv.notify_all();
    for (auto& w : ball_workers) {
        w.join();
    }
    ball_workers.clear();
}

void NarrowPhaseParallelProcessor::process_potential_collisions(UniqueBallPtrPairs potential_collisions) {
    {
        std::lock_guard<std::mutex> guard(tasks_mutex);
        for (auto& bp : potential_collisions) {
            tasks.emplace(std::move(bp));
        }
        tasks_left = tasks.size();
    }
    tasks_cv.notify_all();

    auto self = shared_from_this();
    std::unique_lock<std::mutex> guard1(tasks_mutex);
    tasks_cv.wait(guard1, [self] () {
        return self->tasks_left == 0;
    });
    std::lock_guard<std::mutex> guard2(colliding_mutex);
    std::swap(colliding_pairs, new_colliding_pairs);
    new_colliding_pairs.clear();
}

void NarrowPhaseParallelProcessor::work() {
    while (true) {
        std::optional<BallPtrPair> bp;
        auto self = shared_from_this();
        {
            std::unique_lock<std::mutex> guard(tasks_mutex);
            tasks_cv.wait(guard, [self] () {
                return self->shutdown || !self->tasks.empty();
            });
            if (shutdown) {
                break;
            }
            if (!tasks.empty()) {
                bp = std::move(tasks.front());
                tasks.pop();
            }
        }
        if (bp) {
            const BallPtrPair& bpv = bp.value();
            try {
                process_task(bpv);
            } catch (const std::exception& e) {
                std::cout << "Narrow phase worker failed: " << e.what();
            }
            {
                std::lock_guard<std::mutex> guard(tasks_mutex);
                --tasks_left;
            }
            tasks_cv.notify_all();
        }
    }
}

void NarrowPhaseParallelProcessor::process_task(const BallPtrPair& bp) {
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
