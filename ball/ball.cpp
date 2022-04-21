#include "real-ball.h"

#include <cmath>

namespace Balls {


std::shared_ptr<Ball> make_ball(BallType bt,
                                float x,
                                float y,
                                float r,
                                float dx,
                                float dy) {
    static uint64_t id = -1;
    ++id;
    if (bt == BallType::REAL_BALL) {
        return std::make_shared<RealBall>(id, x, y, r, dx, dy);
    }
    return nullptr;
}

std::shared_ptr<Ball> make_ball(BallType bt, float x, float y)
{
    std::srand(std::time(nullptr));
    uint32_t rv = std::rand();
    static uint16_t angle = 10;
    angle += 100;
    if (angle >= 360) {
        angle = angle % 360;
    }
    float r = MIN_BALL_RADIUS + rv % (MAX_BALL_RADIUS - MIN_BALL_RADIUS + 1);
    float speed = 5 + (rv >> 1) % (MAX_BALL_SPEED - 5);
    float dx = cos(3.14159 * angle / 180) * speed;
    float dy = sin(3.14159 * angle / 180) * speed;

    return make_ball(bt, x, y, r, dx, dy);
}


}
