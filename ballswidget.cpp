#include "ballswidget.h"

#include <QPainter>
#include <QTimer>

namespace Balls {

BallsWidget::BallsWidget(QWidget *parent)
    : QWidget(parent) {

    sm = make_scene_manager(ManagerType::SWEEP_AND_PRUNE);
    assert(sm);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &BallsWidget::render_later);
    timer->start(1000/MAX_FPS);

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

void BallsWidget::resizeEvent(QResizeEvent *e) {
    const QSize& s = e->size();
    sm->set_size(s.width(), s.height());
}

void BallsWidget::render_later() {
    QWidget::update();
}

void BallsWidget::make_frame() {
    {
        std::lock_guard<std::mutex> guard(frames_mutex);
        if (frames.size() > MAX_FRAMES_QUEUE) {
            return;
        }
    }

    Frame balls = sm->get_next_frame();
    if (balls.empty()) {
        return;
    }

    QPixmap pm(size());
    QPainter painter(&pm);
    painter.fillRect(rect(), Qt::white);

    for (auto& b : balls) {
        painter.setPen(Qt::black);
        painter.setBrush(Qt::red);
        painter.drawEllipse(b->x() - b->r(), b->y() - b->r(), b->r() * 2, b->r() * 2);
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
    const QSize& s = size();
    auto b = make_ball(BallType::REAL_BALL, s.width() / 2, s.height() / 2);
    sm->add_ball(std::move(b));
}

void BallsWidget::remove_balls() {
    sm->remove_balls();
    clear_scene();
}


}
