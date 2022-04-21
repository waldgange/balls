#ifndef PREPHASEPARALLELPROCESSOR_H
#define PREPHASEPARALLELPROCESSOR_H

#include <scene-manager.h>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>
#include <queue>


namespace Balls {


class PrePhaseParallelProcessor : public PrePhaseManager, public std::enable_shared_from_this<PrePhaseParallelProcessor>
{
public:
    void start() override;
    void stop() override;
    void process_balls(Frame& f,
                       const float dt,
                       const uint16_t width,
                       const uint16_t height) override;

private:
    std::condition_variable tasks_cv;
    std::mutex tasks_mutex;
     std::vector<std::thread> ball_workers;
     Frame tasks;
     Frame::iterator task_it;
     bool shutdown = false;
    std::condition_variable tasks_counter_cv;
    std::mutex tasks_counter_mutex;
     uint32_t tasks_left;
     float dt;
     uint16_t width;
     uint16_t height;

    void work();
};


}


#endif // PREPHASEPARALLELPROCESSOR_H
