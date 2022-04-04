#ifndef BALL_H
#define BALL_H

#include <cmath>
#include <QLoggingCategory>
#include <QVector2D>
#include <QRect>


namespace Balls {

const uint16_t MAX_FRAMES_QUEUE = 10;
const uint16_t MAX_BALL_SPEED = 10;
const uint16_t MAX_BALL_RADIUS = 30;
const uint16_t MIN_BALL_RADIUS = 5;
// If speed won't be limited, we'll need to always increase process frequency.
// Otherwise we can miss some collisions.

static
void print_vector(const QVector2D& vec, const std::string& name);

struct Ball {
    static uint64_t no_collision_record;
    uint64_t no_collision_cycles = 0;
    QVector2D v;
    float mass = 0.0f;
    float r = 0.0f;
    float x = 0.0f;
    float y = 0.0f;
    float inner_dt = 0.0f;

    Ball (float _r, float _speed, float _direction, float _x, float _y);

    void process(const float dt, const QRect& border);
    void process_border(const QRect& border);
    bool collides(const Ball& other);
    void process_collision(Ball& other, float dt, const QRect& border = QRect(0, 0, 800, 600));
};


}

#endif // BALL_H
