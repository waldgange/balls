#ifndef BALLSWIDGET_H
#define BALLSWIDGET_H

#include "ball.h"

#include <QWidget>
#include <QPaintEvent>
#include <QPixmap>
#include <vector>
#include <unordered_set>
#include <utility>
#include <queue>
#include <mutex>


namespace Balls {

using BallsPair = std::pair<uint16_t, uint16_t>;

struct BallsPairHash {
    size_t operator()(BallsPair p) const noexcept {
        return size_t(p.first) << 16 | p.second;
    }
};

class BallsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BallsWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *) override;
    void showEvent(QShowEvent *) override;

private:
    void process_scene(const float dt = 1.0f / MAX_BALL_SPEED);
    void clear_scene();
    void render_now();

    std::mutex balls_mutex;
     std::vector<Ball> balls;
     std::vector<BallsPair> ball_pairs;
     std::unordered_set<BallsPair, BallsPairHash> colliding_pairs;
    std::mutex frames_mutex;
     std::queue<QPixmap> frames;
     bool processed;

public slots:
    void render_later();
    void make_frame();
    void add_ball();
    void remove_balls();

signals:

};


}

#endif // BALLSWIDGET_H
