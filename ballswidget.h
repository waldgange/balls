#ifndef BALLSWIDGET_H
#define BALLSWIDGET_H

#include "ball.h"

#include <QWidget>
#include <QLoggingCategory>
#include <QPaintEvent>
#include <QPixmap>
#include <vector>
#include <utility>
#include <queue>
#include <mutex>


namespace Balls {

using BallsPair = std::pair<std::uint16_t, std::uint16_t>;

class BallsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BallsWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *) override;
    void showEvent(QShowEvent *) override;

private:
    void render_now();
    void clear_scene();

    std::mutex balls_mutex;
     std::vector<Ball> balls;
     std::vector<BallsPair> ball_pairs;
    std::mutex frames_mutex;
     std::queue<QPixmap> frames;

public slots:
    void render_later();
    void process_scene();

    void add_ball();
    void remove_balls();

signals:

};


}

#endif // BALLSWIDGET_H
