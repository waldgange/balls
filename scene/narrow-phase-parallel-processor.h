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
    virtual void process_potential_collisions(UniqueBallPtrPairs potential_collisions) override;

protected:
    std::mutex colliding_mutex;
     UniqueBallPtrPairs colliding_pairs;
     UniqueBallPtrPairs new_colliding_pairs;

    std::condition_variable tasks_cv;
    std::mutex tasks_mutex;
     std::vector<std::thread> ball_workers;
     std::queue<BallPtrPair> tasks;
     bool shutdown = false;
     uint32_t tasks_left;

    void process_task(const BallPtrPair& bp);
    void work();
    void stop();
};

}

#endif // NARROWPHASEPARALLELPROCESSOR_H
