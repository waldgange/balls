#include "ballswidget.h"

#include <QBackingStore>
#include <QPainter>
#include <QTimer>

namespace Balls {

BallsWidget::BallsWidget(QWidget *parent) : QWidget(parent) {
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &BallsWidget::render_later);
    timer->start(1000/30);

    QTimer *timer1 = new QTimer(this);
    connect(timer1, &QTimer::timeout, this, &BallsWidget::process_scene);
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

void BallsWidget::process_scene() {
    {
        std::lock_guard<std::mutex> g1(balls_mutex);
        std::lock_guard<std::mutex> g2(frames_mutex);
        if (balls.empty() || frames.size() > 99) {
            return;
        }
    }

    QPixmap pm(size());
    QPainter painter(&pm);
    painter.fillRect(rect(), Qt::white);

    {
        std::lock_guard<std::mutex> guard(balls_mutex);
        for (auto& b : balls) {
            b.process(rect());
            painter.setPen(Qt::black);
            painter.setBrush(Qt::red);
            painter.drawEllipse(b.x - b.r, b.y - b.r, b.r * 2, b.r * 2);
        }
        for (auto& b : ball_pairs) {
            balls[b.first].process_collision(balls[b.second]);
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

    std::lock_guard<std::mutex> g2(frames_mutex);
    std::queue<QPixmap> f;
    f.emplace(std::move(pm));
    std::swap(frames, f);
}

void BallsWidget::add_ball() {
    std::lock_guard<std::mutex> guard(balls_mutex);

    balls.emplace_back(30, 10, 250, rect().width() / 2, rect().height() / 2);

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
