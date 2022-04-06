#include "common-scene-manager.h"

#include <map>

namespace Balls {


void CommonSceneManager::add_ball(std::shared_ptr<Ball> b) {
    balls.emplace_back(std::move(b));
}

void CommonSceneManager::remove_balls() {
    balls.clear();
}

Frame CommonSceneManager::get_next_frame() {
    for (int i = 0; i < MAX_BALL_SPEED; ++i) {
        process_scene(1.0f / MAX_BALL_SPEED);
    }
    return balls;
}

void CommonSceneManager::process_scene(const float dt) {
    for (auto& b : balls) {
        b->process(dt, width, height);
    }
    UniqueBallPairs ball_pairs = get_potential_collisions(balls);
    check_potential_collisions(balls, ball_pairs);
    process_collisions(balls, ball_pairs);
}

void CommonSceneManager::process_collisions(Frame& bf, const UniqueBallPairs &ball_pairs) {
    UniqueBallPairs new_colliding_pairs;

    for (auto& b : ball_pairs) {
        std::shared_ptr<Ball>& b1 = bf[b.first];
        std::shared_ptr<Ball>& b2 = bf[b.second];
        BallPair bp = std::make_pair<int, int>(b.first, b.second);
        auto it = colliding_pairs.find(bp);
        bool collided = it != colliding_pairs.end();
        if (!collided) {
            b1->process_collision(b2);
        }
        new_colliding_pairs.emplace(std::move(bp));
    }
    std::swap(colliding_pairs, new_colliding_pairs);
}

void CommonSceneManager::check_potential_collisions(const Frame& bf, UniqueBallPairs &ubp) const {
    for (auto b = ubp.begin(); b != ubp.end();) {
        const std::shared_ptr<Ball>& b1 = bf[b->first];
        const std::shared_ptr<Ball>& b2 = bf[b->second];
        bool colliding = b1->collides(b2);
        if (!colliding) {
            b = ubp.erase(b);
        } else {
            ++b;
        }
    }
}


}
