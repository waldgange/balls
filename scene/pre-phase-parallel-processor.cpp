#include "pre-phase-parallel-processor.h"


namespace Balls {


void PrePhaseParallelProcessor::start() {
    unsigned int threads_count = std::thread::hardware_concurrency() - 1;  // one core for scene thread
    ball_workers.reserve(threads_count);
    auto self = shared_from_this();
    for (std::size_t i = 0; i < threads_count; ++i) {
        ball_workers.emplace_back([self] () {
            self->work();
        });
    }
}

void PrePhaseParallelProcessor::stop() {
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

void PrePhaseParallelProcessor::process_balls(Frame &f,
                                              const float dt_,
                                              const uint16_t width_,
                                              const uint16_t height_) {
    dt = dt_;
    width = width_;
    height = height_;

    tasks_left = f.size();
    tasks = std::ref(f);
    task_it = tasks.begin();
    tasks_cv.notify_all();

    auto self = shared_from_this();
    std::unique_lock<std::mutex> guard(tasks_counter_mutex);
    tasks_counter_cv.wait(guard, [self] () {
        return self->tasks_left == 0;
    });
}

void PrePhaseParallelProcessor::work() {
    auto self = shared_from_this();
    while (true) {
        std::shared_ptr<Ball> bp;
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
            bp->process(dt, width, height);
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


}
