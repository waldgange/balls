#include "real-ball.h"

#include <ctime>

namespace Balls {

std::shared_ptr<Ball> make_ball(const BallType bt,
                                const float x,
                                const float y) {
    if (bt == BallType::REAL_BALL) {
        return std::make_shared<RealBall>(x, y);
    }
    return nullptr;
}



}
