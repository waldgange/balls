#include "mainwidget.h"
#include <QLayout>
#include <QPushButton>
#include <QObject>

namespace Balls {

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    this->resize(920, 620);
    QHBoxLayout* main_lo = new QHBoxLayout(this);

    balls_wgt = new BallsWidget(this);
    balls_wgt->setMinimumSize(600,480);
    main_lo->addWidget(balls_wgt);
    main_lo->setSpacing(10);

    QVBoxLayout* btn_lo = new QVBoxLayout();
    QPushButton *add_btn = new QPushButton("Add");
    QPushButton *clear_btn = new QPushButton("Clear");
    add_btn->setFixedWidth(100);
    clear_btn->setFixedWidth(100);
    btn_lo->addWidget(add_btn);
    btn_lo->addWidget(clear_btn);
    btn_lo->addStretch();
    main_lo->addLayout(btn_lo);

    connect(add_btn, &QPushButton::clicked, balls_wgt, &BallsWidget::add_ball);
    connect(clear_btn, &QPushButton::clicked, balls_wgt, &BallsWidget::remove_balls);
}

MainWidget::~MainWidget()
{
}

}
