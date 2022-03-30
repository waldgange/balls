#include "ball.h"

namespace Balls {


Ball::Ball(float _r, float _speed, float _direction, float _x, float _y)
    : v(cos(3.14159 * _direction / 180) * _speed,
        sin(3.14159 * _direction / 180) * _speed)
    , mass(3.14159 * _r * _r)
    , r(_r)
    , x(_x)
    , y(_y) { }

void Ball::process(const QRect &border) {
    x += v.x();
    y += v.y();
    if (x > border.width() - r) {
        x = border.width() - r;
        if (v.x() > 0) {
            v.setX(-v.x());
        }
    } else if (x < r) {
        x = r;
        if (v.x() < 0) {
            v.setX(-v.x());
        }
    }
    if (y < r) {
        y = r;
        if (v.y() < 0) {
            v.setY(-v.y());
        }
    } else if (y > border.height() - r) {
        y = border.height() - r;
        if (v.y() > 0) {
            v.setY(-v.y());
        }
    }
}

void Ball::process_collision(Balls::Ball &other) {
    if (this == &other) {
        return;
    }
    QVector2D parallel(x - other.x, y - other.y);
    if (parallel.length() == 0
            || (pow(x + v.x() - other.x - other.v.x(), 2) + pow(y + v.y() - other.y - other.v.y(), 2)) > parallel.lengthSquared()
            || parallel.length() > r + other.r) {
        return;
    }
    parallel.normalize();
    float cos_alpha = QVector2D::dotProduct(v, parallel) / v.length();
    float sin_alpha = sqrt(1 - cos_alpha * cos_alpha);
    QVector2D vp = parallel * v.length() * cos_alpha;
    QVector2D vn = parallel * v.length() * sin_alpha;

    float other_cos_alpha = QVector2D::dotProduct(other.v, parallel) / other.v.length();
    float other_sin_alpha = sqrt(1 - other_cos_alpha * other_cos_alpha);
    QVector2D other_vp = parallel * other.v.length() * other_cos_alpha;
    QVector2D other_vn = parallel * other.v.length() * other_sin_alpha;

    QVector2D new_vp = (other_vp * 2 * other.mass + vp * (mass - other.mass)) / (mass + other.mass);
    QVector2D new_other_vp = (vp * 2 * mass + other_vp * (other.mass - mass)) / (mass + other.mass);

    v = vn + new_vp;
    other.v = other_vn + new_other_vp;

    if (std::isnan(v.x()) || std::isnan(v.y())) {
        v.setX(1);
        v.setY(1);
    }
    if (std::isnan(other.v.x()) || std::isnan(other.v.y())) {
        other.v.setX(1);
        other.v.setY(1);
    }
}


}
