#ifndef COMMONSCENEMANAGER_H
#define COMMONSCENEMANAGER_H

#include "scene-manager.h"

#include <mutex>


namespace Balls {

class RealSceneManager : public SceneManager, public std::enable_shared_from_this<RealSceneManager>
{

public:
    RealSceneManager(const std::shared_ptr<PrePhaseManager>&    _ppm,
                     const std::shared_ptr<BroadPhaseManager>&  _bpm,
                     const std::shared_ptr<NarrowPhaseManager>& _npm);
    ~RealSceneManager();

    void start() override;
    void set_size(const uint16_t w, const uint16_t h) override;
    void add_ball(std::shared_ptr<Ball> b) override;
    void remove_balls() override;
    Frame get_next_frame() override;

protected:
    std::mutex scene_mutex;
     Frame frame;
     uint16_t width;
     uint16_t height;

    void process_scene(const float dt = 1.0f / MAX_BALL_SPEED);
};


}

#endif // COMMONSCENEMANAGER_H
