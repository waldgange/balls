#ifndef BALLSWIDGET_H
#define BALLSWIDGET_H

#include "scene/scene-manager.h"

#include <QWidget>
#include <QPaintEvent>
#include <QPixmap>
#include <vector>
#include <unordered_set>
#include <utility>
#include <queue>
#include <mutex>


namespace Balls {

const uint8_t MAX_FPS = 60;
const uint8_t MAX_FRAMES_QUEUE = 100;


class BallsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BallsWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *) override;
    void showEvent(QShowEvent *) override;
    void resizeEvent(QResizeEvent *) override;

private:
    void process_scene(const float dt = 1.0f / MAX_BALL_SPEED);
    UniqueBallPairs get_potential_collisions();

    void clear_scene();
    void render_now();

    std::unique_ptr<SceneManager> sm;

    std::mutex frames_mutex;
     std::queue<QPixmap> frames;

public slots:
    void render_later();
    void make_frame();
    void add_ball();
    void remove_balls();

signals:

};


}

#endif // BALLSWIDGET_H
