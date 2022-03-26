#include "mainwidget.h"
#include <QLayout>
#include <QPushButton>

namespace Balls {

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(920, 620);
    QHBoxLayout* main_lo = new QHBoxLayout(this);

    balls_wgt = new BallsWidget();
    balls_wgt->setFixedSize(800,600);
    main_lo->addWidget(balls_wgt);
    main_lo->setSpacing(10);

    QVBoxLayout* btn_lo = new QVBoxLayout();
    QPushButton *add_btn = new QPushButton("Add");
    QPushButton *clear_btn = new QPushButton("Clear");
    btn_lo->addWidget(add_btn);
    btn_lo->addWidget(clear_btn);
    btn_lo->addStretch();
    main_lo->addLayout(btn_lo);
}

MainWidget::~MainWidget()
{
}

}
