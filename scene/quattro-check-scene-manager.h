#ifndef QUATTROCHECKSCENEMANAGER_H
#define QUATTROCHECKSCENEMANAGER_H

#include "common-scene-manager.h"

namespace Balls {

class QuattroCheckSceneManager : public CommonSceneManager
{
public:
    void add_ball(std::shared_ptr<Ball> b) override;
    void remove_balls() override;
protected:
    UniqueBallPairs get_potential_collisions(const Frame& bf) const override;
private:
    UniqueBallPairs ball_pairs;
};

}

#endif // QUATTROCHECKSCENEMANAGER_H
