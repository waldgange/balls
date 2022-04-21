#ifndef REALBALL_H
#define REALBALL_H

#include "ball.h"

#include <QVector2D>
#include <shared_mutex>

namespace Balls {

class RealBall : public Ball
{
public:
    RealBall(uint64_t id,
             float x,
             float y,
             float r,
             float dx,
             float dy);

    void process(const float dt, const uint16_t width, const uint16_t height) override;
    bool collides(const std::shared_ptr<Ball>& other) const override;
    void process_collision(const std::shared_ptr<Ball>& other) override;

    uint32_t id() const override;
    float x() const override;
    float y() const override;
    float r() const override;

protected:
    void process_border(const uint16_t width, const uint16_t height);

    mutable std::shared_mutex m;
     QVector2D v;
     uint32_t _id;
     float _x;
     float _y;
     float _r;
     float _mass;
};

}

#endif // REALBALL_H
