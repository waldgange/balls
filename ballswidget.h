#ifndef BALLSWIDGET_H
#define BALLSWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <list>
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

    Ball (uint8_t _r, uint8_t _speed, uint16_t _direction, uint16_t _x = 0, uint16_t _y = 0)
        : x(_x)
        , y(_y)
        , speed(_speed)
        , r(_r) {
        dx = speed * cos(_direction * 3.14159 / 180);
        dy = speed * sin(_direction * 3.14159 / 180);
    }

    uint8_t mass() const {
        return 3.14 * r * r;  // proportional to square
    }

    void process(const QRect& border) {
        x += dx;
        y += dy;
        if (x > border.width() - r || x < r) {
            dx = -dx;
            x += dx;
        }
        if (y < r || y > border.height() - r) {
            dy = -dy;
            y += dy;
        }
    }

    void process_collision(Ball& other) {
        int16_t xx = x - other.x;
        int16_t yy = y - other.y;
        uint16_t path = sqrt(xx * xx + yy * yy);  // xx + yy / (2 * xx)
        if (path > r + other.r) {
            return;
        }

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

    std::list<Ball> balls;
    std::queue<QPixmap> frames;//, dummys;
    std::mutex frames_mutex;//, dummys_mutex;

public slots:
    void render_later();
    void process_scene();

signals:

};


}

#endif // BALLSWIDGET_H
