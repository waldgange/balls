#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "ballswidget.h"
#include <QWidget>

namespace Balls {

class MainWidget : public QWidget
{
    Q_OBJECT

    QWidget* balls_wgt;
public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
};

}

#endif // MAINWIDGET_H
