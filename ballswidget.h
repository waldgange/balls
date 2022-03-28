#ifndef BALLSWIDGET_H
#define BALLSWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <vector>
#include <utility>
#include <queue>
#include <mutex>
#include <cmath>
#include <QLoggingCategory>

namespace Balls {

struct Ball {
    uint16_t x = 0;
    uint16_t y = 0;
    int16_t dx = 0;
    int16_t dy = 0;
    uint8_t speed = 0;
    const uint8_t r = 0;
    const uint8_t mass = 0;

    Ball (uint8_t _r, uint8_t _speed, uint16_t _direction, uint16_t _x = 0, uint16_t _y = 0)
        : x(_x)
        , y(_y)
        , speed(_speed)
        , r(_r)
        , mass(3.14 * r * r / 100) {
        dx = speed * cos(_direction * 3.14159 / 180);
        dy = speed * sin(_direction * 3.14159 / 180);
    }

    void process(const QRect& border) {
        x += dx;
        y += dy;
        if (x > border.width() - r) {
            x -= x + r - border.width();
            dx = -dx;
        } else if (x < r) {
            x += r - x;
            dx = -dx;
        }
        if (y < r) {
            y += r - y;
            dy = -dy;
        } else if (y > border.height() - r) {
            y -= y + r - border.height();
            dy = -dy;
        }
    }

    void process_collision(Ball& other) {
        if (this == &other) {
            return;
        }
        int16_t xx = x - other.x;
        int16_t yy = y - other.y;
        int16_t next_xx = xx + dx - other.dx;
        int16_t next_yy = yy + dy - other.dy;
        uint16_t path = xx * xx + yy * yy;  // xx + yy / (2 * xx)
        uint16_t next_path = next_xx * next_xx + next_yy * next_yy;
        if (path == 0 || next_path > path || path > (r + other.r) * (r + other.r)) {
            return;
        }
        path = sqrt(path);
        double nx = -xx / path;
        double ny = -yy / path;

        double p = 2 * (dx * nx + dy * ny - other.dx * nx - other.dy * ny) /
                (mass + other.mass);
        dx = dx - p * mass * nx;
        dy = dy - p * mass * ny;
        other.dx = other.dx + p * other.mass * nx;
        other.dy = other.dy + p * other.mass * ny;
    }
};

class BallsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BallsWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void render_now(const QRect& rect);

    using BallsPair = std::pair<std::uint16_t, std::uint16_t>;

    std::vector<Ball> balls;
    std::vector<BallsPair> ball_pairs;
    std::queue<QPixmap> frames;
    std::mutex frames_mutex;
public slots:
    void render_later();
    void process_scene();

signals:

};


}

#endif // BALLSWIDGET_H
