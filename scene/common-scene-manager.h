#ifndef COMMONSCENEMANAGER_H
#define COMMONSCENEMANAGER_H

#include "scene-manager.h"

namespace Balls {

class CommonSceneManager : public SceneManager
{
public:
    void add_ball(std::shared_ptr<Ball> b) override;
    void remove_balls() override;
    Frame get_next_frame() override;

protected:
    Frame balls;
    UniqueBallPairs colliding_pairs;

    virtual UniqueBallPairs get_potential_collisions(const Frame& bf) const = 0;

    virtual void check_potential_collisions(const Frame& bf, UniqueBallPairs& ubp) const;
    virtual void process_collisions(Frame& bf, const UniqueBallPairs& ubp);
    virtual void process_scene(const float dt = 1.0f / MAX_BALL_SPEED);

};

}

#endif // COMMONSCENEMANAGER_H
