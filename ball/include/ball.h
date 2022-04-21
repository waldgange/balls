#ifndef BALL_H
#define BALL_H

#include <memory>


namespace Balls {

// If speed won't be limited, we'll need to always increase process frequency.
// Otherwise we can miss some collisions.
const uint16_t MAX_BALL_SPEED = 10;
const uint16_t MAX_BALL_RADIUS = 30;
const uint16_t MIN_BALL_RADIUS = 5;
const uint32_t MAX_BALL_COUNT = 100000;

class Ball {
public:
    virtual ~Ball() {};

    virtual void process(const float dt, const uint16_t width, const uint16_t height) = 0;
    virtual void process_collision(const std::shared_ptr<Ball>& other) = 0;
    virtual bool collides(const std::shared_ptr<Ball>& other) const = 0;

    virtual uint32_t id() const = 0;
    virtual float x() const = 0;
    virtual float y() const = 0;
    virtual float r() const = 0;
};

enum class BallType {
    REAL_BALL
};

std::shared_ptr<Ball> make_ball(BallType bt,
                                float x,
                                float y,
                                float r,
                                float dx,
                                float dy);

std::shared_ptr<Ball> make_ball(BallType bt,
                                float x,
                                float y);

}

#endif // BALL_H
