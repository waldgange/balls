#include "real-ball.h"

#include <cmath>
#include <ctime>
#include <mutex>

namespace Balls {


RealBall::RealBall(uint64_t id,
                   float x,
                   float y,
                   float r,
                   float dx,
                   float dy)
    : _id(id)
    , _x(x)
    , _y(y)
    , _r(r)
    , _mass(3.14159 * r * r) {
    v.setX(dx);
    v.setY(dy);
}

void RealBall::process(const float dt,
                       const uint16_t width,
                       const uint16_t height) {
    std::unique_lock lock(m);

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

bool RealBall::collides(const std::shared_ptr<Ball>& o) const {
    RealBall* other = dynamic_cast<RealBall*>(o.get());
    if (!other || other == this) {
        throw std::runtime_error("Invvalid argument type");
    }
    std::shared_lock lock1(m, std::defer_lock);
    std::shared_lock lock2(other->m, std::defer_lock);
    std::lock(lock1, lock2);
    return QVector2D(_x - other->x(), _y - other->y()).length() < _r + other->r();
}

void RealBall::process_collision(const std::shared_ptr<Ball>& o) {
    RealBall* other = dynamic_cast<RealBall*>(o.get());
    if (!other || other == this) {
        throw std::runtime_error("Invvalid argument type");
    }

    std::unique_lock lock1(m, std::defer_lock);
    std::unique_lock lock2(other->m, std::defer_lock);
    std::lock(lock1, lock2);

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

    QVector2D new_vp = (2.0f * other_vp * other->_mass + vp * (_mass - other->_mass)) / (_mass + other->_mass);
    QVector2D new_other_vp = (2.0f * vp * _mass + other_vp * (other->_mass - _mass)) / (_mass + other->_mass);

    v = vn + new_vp;
    other->v = other_vn + new_other_vp;
    fix_velocity(v);
    fix_velocity(other->v);

    return;
}

uint32_t RealBall::id() const {
    return _id;
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
