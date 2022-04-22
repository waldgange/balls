#ifndef BALLSWIDGET_H
#define BALLSWIDGET_H

#include "scene-manager.h"

#include <QWidget>
#include <QPaintEvent>
#include <QPixmap>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>


namespace Balls {

const uint8_t MAX_FPS = 60;
const uint8_t MAX_FRAMES_QUEUE = 100;


class BallsWidget : public QWidget, public QEnableSharedFromThis<BallsWidget>
{
    Q_OBJECT
public:
    explicit BallsWidget(QWidget *parent = nullptr);
    ~BallsWidget();

protected:
    void paintEvent(QPaintEvent *) override;
    void showEvent(QShowEvent *) override;
    void resizeEvent(QResizeEvent *) override;

private:
    void start();
    void work();
    void stop();
    void clear_scene();
    void render_now();
    QPixmap make_frame(const Frame& balls);

    std::shared_ptr<SceneManager> sm;

    std::thread scene_worker;

    std::mutex scene_mutex;
    std::condition_variable fcv;
     std::queue<QPixmap> frames;
     bool shutdown;

public slots:
    void render_later();
    void add_ball();
    void remove_balls();
    void set_pre(int n);
    void set_broad(int n);
    void set_narrow(int n);

signals:

};


}

#endif // BALLSWIDGET_H
