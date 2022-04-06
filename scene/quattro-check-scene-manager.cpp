#include "quattro-check-scene-manager.h"

namespace Balls {


void QuattroCheckSceneManager::add_ball(std::shared_ptr<Ball> b) {
    CommonSceneManager::add_ball(std::move(b));
    for (int i = 0; i < balls.size() - 1; ++i) {
        ball_pairs.emplace(i, balls.size() - 1);
    }
}

void QuattroCheckSceneManager::remove_balls() {
    ball_pairs.clear();
    CommonSceneManager::remove_balls();
}

UniqueBallPairs QuattroCheckSceneManager::get_potential_collisions(const Frame &bf) const {
    return ball_pairs;
}


}
