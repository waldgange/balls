#include "ballswidget.h"

#include <QPainter>
#include <QTimer>
#include <QTime>

namespace Balls {

BallsWidget::BallsWidget(QWidget *parent)
    : QWidget(parent)
    , shutdown(true) {
    sm = make_scene_manager(PrePhaseType::SEQUENT,
                            BroadPhaseType::QWATTRO,
                            NarrowPhaseType::SEQUENT);
    assert(sm);
    sm->start();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &BallsWidget::render_later);
    timer->start(1000/MAX_FPS);
}

BallsWidget::~BallsWidget() {
    stop();
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

void BallsWidget::start() {
    {
        std::unique_lock<std::mutex> guard(scene_mutex);
        if (!shutdown) {
            return;
        }
        shutdown = false;
        scene_worker = std::thread(&BallsWidget::work, this);
    }
    fcv.notify_one();
}

void BallsWidget::stop() {
    {
        std::unique_lock<std::mutex> guard(scene_mutex);
        if (shutdown) {
            return;
        }
        shutdown = true;
    }
    fcv.notify_one();
    scene_worker.join();
}

void BallsWidget::work() {
    while (true) {
        std::unique_lock<std::mutex> guard(scene_mutex);
        fcv.wait(guard, [this] () {
            return shutdown || frames.size() < MAX_FRAMES_QUEUE;
        });
        if (shutdown) {
            return;
        }
        const Frame& f = sm->get_next_frame();
        frames.emplace(make_frame(f));
    }
}

void BallsWidget::render_later() {
    QWidget::update();
}

QPixmap BallsWidget::make_frame(const Frame& balls) {
    QPixmap pm(size());
    QPainter painter(&pm);
    const QRect& r = rect();
    painter.fillRect(r, Qt::white);

    for (auto& b : balls) {
        painter.setPen(Qt::black);
        painter.setBrush(Qt::red);
        painter.drawEllipse(b->x() - b->r(),
                            b->y() - b->r(),
                            b->r() * 2,
                            b->r() * 2);
    }

    painter.end();

    return pm;
}

void BallsWidget::render_now() {
    static QTime begin = QTime::currentTime();
    static uint8_t fps = 0;
    static uint8_t f = 0;

    int diff = begin.msecsTo(QTime::currentTime());
    if (diff > 1000) {
        fps = f;
        f = 0;
        begin = QTime::currentTime();
    }

    static QPixmap pm;
    {
        std::lock_guard<std::mutex> guard(scene_mutex);
        if (!frames.empty()) {
            pm = std::move(frames.front());
            frames.pop();
            ++f;
        }
    }
    fcv.notify_all();
    QPainter painter(this);
    painter.drawPixmap(0, 0, pm);

    painter.setPen(Qt::darkYellow);
    QFont font = painter.font();
    font.setPixelSize(50);
    font.setWeight(QFont::Bold);
    painter.setFont(font);
    painter.drawText(QRect(pm.width() - 202, 0, 200, 50),
                     Qt::AlignRight,
                     QString("%1 FPS").arg(fps));
    painter.end();
}

void BallsWidget::clear_scene() {
    QPixmap pm(size());
    QPainter painter(&pm);
    painter.fillRect(rect(), Qt::white);
    painter.end();

    std::lock_guard<std::mutex> guard(scene_mutex);
    std::queue<QPixmap> f;
    f.emplace(std::move(pm));
    std::swap(frames, f);
}

void BallsWidget::add_ball() {
    start();
    const QSize& s = size();
    auto b = make_ball(BallType::REAL_BALL, s.width() / 2, s.height() / 2);
    sm->add_ball(std::move(b));
}

void BallsWidget::remove_balls() {
    stop();
    clear_scene();
    sm->remove_balls();
}

void BallsWidget::set_pre(int n) {
    auto ppm = make_pre_manager(static_cast<PrePhaseType>(n));
    if (ppm) {
        stop();
        sm->set_pre_phase_manager(std::move(ppm));
        start();
    }
}

void BallsWidget::set_broad(int n) {
    auto bpm = make_broad_manager(static_cast<BroadPhaseType>(n));
    if (bpm) {
        stop();
        sm->set_broad_phase_manager(std::move(bpm));
        start();
    }
}

void BallsWidget::set_narrow(int n) {
    auto npm = make_narrow_manager(static_cast<NarrowPhaseType>(n));
    if (npm) {
        stop();
        sm->set_narrow_phase_manager(std::move(npm));
        start();
    }
}


}
