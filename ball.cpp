#include "ball.h"

namespace Balls {


Ball::Ball(float _r, float _speed, float _direction, float _x, float _y)
    : v(cos(3.14159 * _direction / 180) * _speed,
        sin(3.14159 * _direction / 180) * _speed)
    , mass(3.14159 * _r * _r)
    , r(_r)
    , x(_x)
    , y(_y) { }

void Ball::process(const float dt, const QRect &border) {
    x += v.x() * dt;
    y += v.y() * dt;

    process_border(border);
}

void Ball::process_border(const QRect &border) {
    if (x + r > border.width()) {
        x = border.width() - r;
        if (v.x() > 0) {
            v.setX(-v.x());
        }
    } else if (x - r < 0) {
        x = r;
        if (v.x() < 0) {
            v.setX(-v.x());
        }
    }
    if (y - r < 0) {
        y = r;
        if (v.y() < 0) {
            v.setY(-v.y());
        }
    } else if (y + r > border.height()) {
        y = border.height() - r;
        if (v.y() > 0) {
            v.setY(-v.y());
        }
    }
}

bool Ball::collides(const Ball &other) {
    return QVector2D(x - other.x, y - other.y).length() < r + other.r;
}

void Ball::process_collision(Balls::Ball &other, float dt, const QRect& border) {
    QVector2D parallel(x - other.x, y - other.y);
    float path = parallel.length();

    if (path < 5.0f || std::isnan(path) || std::isnan(parallel.x()) || std::isnan(parallel.y())) {
        process(dt, border);
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
    if (other.v.length() != 0) {
        float other_cos_alpha = QVector2D::dotProduct(other.v, parallel) / other.v.length();
        float other_sin_alpha = QVector2D::dotProduct(other.v, normal) / other.v.length();
        other_vp = parallel * other.v.length() * other_cos_alpha;
        other_vn = normal * other.v.length() * other_sin_alpha;
    }

    QVector2D new_vp = (2.0f * other_vp * other.mass + vp * (mass - other.mass)) / (mass + other.mass);
    QVector2D new_other_vp = (2.0f * vp * mass + other_vp * (other.mass - mass)) / (mass + other.mass);

    v = vn + new_vp;
    other.v = other_vn + new_other_vp;
    fix_velocity(v);
    fix_velocity(other.v);

    no_collision_cycles = 0;
    other.no_collision_cycles = 0;

    return;
}

void print_vector(const QVector2D &vec, const std::string &name) {
    qDebug() << name.c_str() << ":\t" << vec << "\t=\t" << vec.length()
             << "\t" << (int)(atanf(vec.y() / vec.x()) * 180 / 3.14) << " degree";
}


}
