#include "ballswidget.h"

#include <QPainter>
#include <QTimer>
#include <cstdlib>
#include <iostream>
#include <ctime>

namespace Balls {

BallsWidget::BallsWidget(QWidget *parent)
    : QWidget(parent) {
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

    UniqueBallPairs ball_pairs = get_potential_collisions();
    UniqueBallPairs new_colliding_pairs;

    for (auto& b : ball_pairs) {
        Ball& b1 = balls[b.first];
        Ball& b2 = balls[b.second];
        BallPair bp = std::make_pair<int, int>(b.first, b.second);
        auto it = colliding_pairs.find(bp);
        bool collided = it != colliding_pairs.end();
        bool colliding = b1.collides(b2);
        if (colliding) {
            if (!collided) {
                b1.process_collision(b2, dt, r);
            }
            new_colliding_pairs.emplace(std::move(bp));
        }
    }
    std::swap(colliding_pairs, new_colliding_pairs);
}

UniqueBallPairs BallsWidget::get_potential_collisions() {
    // "Sweep and Prune" algorythm
    // Breaking O(n^2) complexity of checking all the pairs
    std::multimap<float, int> balls_x, balls_y;
    for(int i = 0; i < balls.size(); ++i) {
        balls_x.emplace(balls[i].x - balls[i].r, i);
        balls_y.emplace(balls[i].y - balls[i].r, i);
    }

    auto filter_pairs = [&] (std::multimap<float, int>& balls_on_axle) -> UniqueBallPairs {
        UniqueBallPairs r;

        auto l_it = balls_on_axle.begin();
        auto r_it = l_it;
        while (l_it != balls_on_axle.end()) {
            if (l_it == r_it) {
                ++r_it;
                continue;
            }
            if (r_it == balls_on_axle.end() || l_it->first + balls[l_it->second].r * 2 < r_it->first) {
                ++l_it;
                r_it = l_it;
                continue;
            }
            if (l_it->second < r_it->second) {
                r.emplace(l_it->second, r_it->second);
            } else {
                r.emplace(r_it->second, l_it->second);
            }
            ++r_it;
        }

        return r;
    };

    UniqueBallPairs result;
    const UniqueBallPairs bx = filter_pairs(balls_x);
    const UniqueBallPairs by = filter_pairs(balls_y);
    for (const auto& b : bx) {
        if (by.find(b) != by.end()) {
            result.emplace(b);
        }
    }

    return result;
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
        for (int i = 0; i < MAX_BALL_SPEED; ++i) {
            process_scene(1.0f / MAX_BALL_SPEED);
        }
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

    Ball b(MIN_BALL_RADIUS + rv % (MAX_BALL_RADIUS - MIN_BALL_RADIUS + 1),
           5 + (rv >> 1) % (MAX_BALL_SPEED - 5),
           angle,
           rect().width() / 2,
           rect().height() / 2);

    std::lock_guard<std::mutex> guard(balls_mutex);

    balls.emplace_back(std::move(b));
}

void BallsWidget::remove_balls() {
    std::lock_guard<std::mutex> g1(balls_mutex);
    balls.clear();
    clear_scene();
}


}
