#ifndef NARROWPHASEPARALLELPROCESSOR_H
#define NARROWPHASEPARALLELPROCESSOR_H

#include "scene-manager.h"

#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>
#include <queue>

namespace Balls {


class NarrowPhaseParallelProcessor : public NarrowPhaseManager, public std::enable_shared_from_this<NarrowPhaseParallelProcessor>
{

public:
    ~NarrowPhaseParallelProcessor();

    void start() override;
    virtual void process_potential_collisions(UniqueBallPairs potential_collisions) override;

protected:
    std::mutex colliding_mutex;
     UniqueBallPairs colliding_pairs;
     UniqueBallPairs new_colliding_pairs;

    std::condition_variable tasks_cv;
    std::mutex tasks_mutex;
     std::vector<std::thread> ball_workers;
     UniqueBallPairs tasks;
     UniqueBallPairs::iterator task_it;
     bool shutdown = false;
    std::condition_variable tasks_counter_cv;
    std::mutex tasks_counter_mutex;
     uint32_t tasks_left;

    void process_task(const BallPair& bp);
    void work();
    void stop();
};

}

#endif // NARROWPHASEPARALLELPROCESSOR_H
