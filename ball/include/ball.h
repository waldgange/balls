#ifndef BALL_H
#define BALL_H

#include <stdint.h>
#include <memory>


namespace Balls {

const uint16_t MAX_BALL_SPEED = 10;
const uint16_t MAX_BALL_RADIUS = 30;
const uint16_t MIN_BALL_RADIUS = 5;
// If speed won't be limited, we'll need to always increase process frequency.
// Otherwise we can miss some collisions.

class Ball {
public:
    virtual ~Ball() {}

    virtual void process(const float dt, const uint16_t width, const uint16_t height) = 0;
    virtual void process_border(const uint16_t width, const uint16_t height) = 0;
    virtual bool collides(const std::shared_ptr<Ball>& other) const = 0;
    virtual void process_collision(const std::shared_ptr<Ball>& other) = 0;

    virtual float x() const = 0;
    virtual float y() const = 0;
    virtual float r() const = 0;
};

enum class BallType {
    REAL_BALL
};

std::shared_ptr<Ball> make_ball(const BallType bt, const float x, const float y);

}

#endif // BALL_H