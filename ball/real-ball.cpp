#include "real-ball.h"

#include <cmath>
#include <ctime>

namespace Balls {


RealBall::RealBall(float x, float y) : Ball()
  , _x(x)
  , _y(y) {
    std::srand(std::time(nullptr));
    uint32_t rv = std::rand();
    static uint16_t angle = 10;
    angle += 100;
    if (angle >= 360) {
        angle = angle % 360;
    }
    _r = MIN_BALL_RADIUS + rv % (MAX_BALL_RADIUS - MIN_BALL_RADIUS + 1);
    mass = 3.14159 * _r * _r;
    float speed = 5 + (rv >> 1) % (MAX_BALL_SPEED - 5);
    v.setX(cos(3.14159 * angle / 180) * speed);
    v.setY(sin(3.14159 * angle / 180) * speed);
}

void RealBall::process(const float dt,
                       const uint16_t width,
                       const uint16_t height) {
    _x += v.x() * dt;
    _y += v.y() * dt;

    process_border(width, height);
}

void RealBall::process_border(const uint16_t width,
                              const uint16_t height) {
    if (_x + _r > width) {
        _x = width - _r;
        if (v.x() > 0) {
            v.setX(-v.x());
        }
    } else if (_x - _r < 0) {
        _x = _r;
        if (v.x() < 0) {
            v.setX(-v.x());
        }
    }
    if (_y - _r < 0) {
        _y = _r;
        if (v.y() < 0) {
            v.setY(-v.y());
        }
    } else if (_y + _r > height) {
        _y = height - _r;
        if (v.y() > 0) {
            v.setY(-v.y());
        }
    }
}

bool RealBall::collides(const std::shared_ptr<Ball>& other) const {
    return QVector2D(_x - other->x(), _y - other->y()).length() < _r + other->r();
}

void RealBall::process_collision(const std::shared_ptr<Ball>& o) {
    RealBall* other = dynamic_cast<RealBall*>(o.get());
    if (!other) {
        return;
    }

    QVector2D parallel(_x - other->_x, _y - other->_y);
    float path = parallel.length();

    if (path < 5.0f || std::isnan(path) || std::isnan(parallel.x()) || std::isnan(parallel.y())) {
        return;
    }

    parallel.normalize();
    QVector2D normal(-parallel.y(), parallel.x());

    auto fix_velocity = [] (QVector2D& vec) {
        if (std::isnan(vec.x()) || std::isnan(vec.y())) {
            vec.setX(0.0f);
            vec.setY(0.0f);
        } else if (vec.length() > MAX_BALL_SPEED) {
            vec.normalize();
            vec *= MAX_BALL_SPEED;
        }
    };

    QVector2D vp, vn;
    if (v.length() != 0) {
        float cos_alpha = QVector2D::dotProduct(v, parallel) / v.length();
        float sin_alpha = QVector2D::dotProduct(v, normal) / v.length();
        vp = parallel * v.length() * cos_alpha;
        vn = normal * v.length() * sin_alpha;
    }
    QVector2D other_vp, other_vn;
    if (other->v.length() != 0) {
        float other_cos_alpha = QVector2D::dotProduct(other->v, parallel) / other->v.length();
        float other_sin_alpha = QVector2D::dotProduct(other->v, normal) / other->v.length();
        other_vp = parallel * other->v.length() * other_cos_alpha;
        other_vn = normal * other->v.length() * other_sin_alpha;
    }

    QVector2D new_vp = (2.0f * other_vp * other->mass + vp * (mass - other->mass)) / (mass + other->mass);
    QVector2D new_other_vp = (2.0f * vp * mass + other_vp * (other->mass - mass)) / (mass + other->mass);

    v = vn + new_vp;
    other->v = other_vn + new_other_vp;
    fix_velocity(v);
    fix_velocity(other->v);

    return;
}

float RealBall::x() const {
    return _x;
}

float RealBall::y() const {
    return _y;
}

float RealBall::r() const {
    return _r;
}


}
