#ifndef BALL_H
#define BALL_H

#include <cmath>
#include <QVector2D>
#include <QRect>


namespace Balls {


struct Ball {
    QVector2D v;
    float mass = 0;
    float r = 0;
    float x = 0;
    float y = 0;

    Ball (float _r, float _speed, float _direction, float _x = 0, float _y = 0);

    void process(const QRect& border);

    void process_collision(Ball& other);
};


}

#endif // BALL_H
