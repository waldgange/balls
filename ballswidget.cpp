#include "ballswidget.h"

#include <QBackingStore>
#include <QPainter>
#include <QTimer>

namespace Balls {

BallsWidget::BallsWidget(QWidget *parent) : QWidget(parent) {
    balls.emplace_back(70, 10, 30, rect().width() / 2, rect().height() / 2);
    balls.emplace_back(70, 10, 140, rect().width() / 2 + 100, rect().height() / 2);
//    balls.emplace_back(70, 10, 210, rect().width() / 2, rect().height() / 2);
//    balls.emplace_back(70, 10, 320, rect().width() / 2, rect().height() / 2);

    for (std::uint16_t i = 0; i < balls.size(); ++i) {
        for (std::uint16_t j = i + 1; i < balls.size(); ++i) {
            ball_pairs.emplace_back(i, j);
        }
    }

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &BallsWidget::render_later);
    timer->start(1000/30);

    QTimer *timer1 = new QTimer(this);
    connect(timer1, &QTimer::timeout, this, &BallsWidget::process_scene);
    timer1->start(1000/60);
}

void BallsWidget::paintEvent(QPaintEvent *event) {
    render_now(event->rect());
}

void BallsWidget::render_later() {
    QWidget::update();
}

void BallsWidget::process_scene() {
    QPixmap pm(size());

    QPainter painter(&pm);
    painter.fillRect(rect(), Qt::white);

    for (auto& b : balls) {
        b.process(rect());
        painter.setPen(Qt::black);
        painter.setBrush(Qt::red);
        painter.drawEllipse(b.x - b.r, b.y - b.r, b.r * 2, b.r * 2);
    }
    for (auto& b : ball_pairs) {
        balls[b.first].process_collision(balls[b.second]);
    }

    painter.end();
    frames.emplace(std::move(pm));
}

void BallsWidget::render_now(const QRect& rect) {
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

//    {
//        std::lock_guard<std::mutex> guard(dummys_mutex);
//        dummys.emplace(std::move(pm));
//    }
}


}
