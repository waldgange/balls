#ifndef REALBALL_H
#define REALBALL_H

#include "ball.h"

#include <QVector2D>

namespace Balls {

class RealBall : public Ball
{
public:
    RealBall(float _x, float _y);

    void process(const float dt, const uint16_t width, const uint16_t height) override;
    void process_border(const uint16_t width, const uint16_t height) override;
    bool collides(const std::shared_ptr<Ball>& other) const override;
    void process_collision(const std::shared_ptr<Ball>& other) override;

    float x() const override;
    float y() const override;
    float r() const override;

protected:
    QVector2D v;
    float mass = 0.0f;
    float _r = 0.0f;
    float _x = 0.0f;
    float _y = 0.0f;
};

}

#endif // REALBALL_H
