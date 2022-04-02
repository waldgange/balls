#include "ballswidget.h"

#include <QPainter>
#include <QTimer>
#include <cstdlib>
#include <iostream>
#include <ctime>

namespace Balls {

BallsWidget::BallsWidget(QWidget *parent)
    : QWidget(parent)
    , processed(true) {

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &BallsWidget::render_later);
    timer->start(1000/30);

    QTimer *timer1 = new QTimer(this);
    connect(timer1, &QTimer::timeout, this, &BallsWidget::make_frame);
    timer1->start(1000/100);
}

void BallsWidget::paintEvent(QPaintEvent* ) {
    render_now();
}

void BallsWidget::showEvent(QShowEvent *) {
    clear_scene();
}

void BallsWidget::render_later() {
    QWidget::update();
}

void BallsWidget::process_scene(const float dt) {
    const QRect& r = rect();
    for (auto& b : balls) {
        b.process(dt, r);
    }
    for (auto& b : ball_pairs) {
        Ball& b1 = balls[b.first];
        Ball& b2 = balls[b.second];
        BallsPair bp = std::make_pair<int, int>(b.first, b.second);
        auto it = colliding_pairs.find(bp);
        bool collided = it != colliding_pairs.end();
        bool colliding = b1.collides(b2);
        if (colliding && !collided) {
            b1.process_collision(b2, dt, r);
            colliding_pairs.emplace(std::move(bp));
        } else if (!colliding && collided) {
            colliding_pairs.erase(it);
        }
    }
}

void BallsWidget::make_frame() {
    {
        std::lock_guard<std::mutex> guard(frames_mutex);
        if (frames.size() > MAX_FRAMES_QUEUE) {
            return;
        }
    }

    QPixmap pm(size());
    QPainter painter(&pm);
    painter.fillRect(rect(), Qt::white);

    {
        std::lock_guard<std::mutex> guard(balls_mutex);
        if (balls.empty()) {
            return;
        }
        float max_speed = 1.0f;
        for (const auto& b : balls) {
            max_speed = std::max(max_speed, abs(b.v.length()));
        }
        for (int i = 0; i < int(max_speed); ++i) {
            process_scene(1.0f / max_speed);
        }
        processed = true;
        for (auto& b : balls) {
            painter.setPen(Qt::black);
            painter.setBrush(Qt::red);
            painter.drawEllipse(b.x - b.r, b.y - b.r, b.r * 2, b.r * 2);
        }
    }

    painter.end();

    std::lock_guard<std::mutex> guard(frames_mutex);
    frames.emplace(std::move(pm));
}

void BallsWidget::render_now() {
    static QPixmap pm;
    {
        std::lock_guard<std::mutex> guard(frames_mutex);
        if (!frames.empty()) {
            pm = std::move(frames.front());
            frames.pop();
        }
    }
    QPainter painter(this);
    painter.drawPixmap(0, 0, pm);
    painter.end();
}

void BallsWidget::clear_scene() {
    QPixmap pm(size());
    QPainter painter(&pm);
    painter.fillRect(rect(), Qt::white);
    painter.end();

    std::lock_guard<std::mutex> guard(frames_mutex);
    std::queue<QPixmap> f;
    f.emplace(std::move(pm));
    std::swap(frames, f);
}

void BallsWidget::add_ball() {
    std::srand(std::time(nullptr));
    uint32_t rv = std::rand();
    static uint16_t angle = 10;
    angle += 100;
    if (angle >= 360) {
        angle = angle % 360;
    }

    Ball b(5 + rv % 26,
           5 + (rv >> 1) % 21,
           angle,
           rect().width() / 2,
           rect().height() / 2);

    std::lock_guard<std::mutex> guard(balls_mutex);
    if (!processed) {
        return;
    }
    processed = false;

    balls.emplace_back(std::move(b));

    for (int i = 0; i < balls.size() - 1; ++i) {
        ball_pairs.emplace_back(i, balls.size() - 1);
    }
}

void BallsWidget::remove_balls() {
    std::lock_guard<std::mutex> g1(balls_mutex);
    ball_pairs.clear();
    balls.clear();
    clear_scene();
}


}
