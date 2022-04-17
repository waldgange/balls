#include "real-ball.h"

#include <ctime>

namespace Balls {

std::shared_ptr<Ball> make_ball(const BallType bt,
                                const float x,
                                const float y) {
    static uint64_t id = -1;
    ++id;
    if (bt == BallType::REAL_BALL) {
        return std::make_shared<RealBall>(id, x, y);
    }
    --id;
    return nullptr;
}

Ball::~Ball() {}

}
