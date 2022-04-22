#include "mainwidget.h"
#include <QButtonGroup>
#include <QGroupBox>
#include <QLayout>
#include <QObject>
#include <QPushButton>
#include <QRadioButton>

namespace Balls {

static
std::pair<QGroupBox*, QButtonGroup*> make_gb(const std::string& label, const std::vector<std::string>& butons) {
    if (butons.size() < 2) {
        throw std::runtime_error("too few buttons");
    }
    QVBoxLayout* vbl = new QVBoxLayout();
    QButtonGroup* bg = new QButtonGroup();
    bg->setExclusive(true);

    for (int i = 0; i < butons.size(); ++i) {
        QRadioButton* b = new QRadioButton(butons[i].c_str());
        bg->addButton(b, i);
        vbl->addWidget(b);
    }
    bg->button(0)->setChecked(true);

    QGroupBox* gb = new QGroupBox(label.c_str());
    gb->setLayout(vbl);

    return std::make_pair(gb, bg);
}


MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    this->resize(920, 620);
    QHBoxLayout* main_lo = new QHBoxLayout(this);

    balls_wgt = new BallsWidget(this);
    balls_wgt->setMinimumSize(600,480);
    main_lo->addWidget(balls_wgt);
    main_lo->setSpacing(10);

    QPushButton *add_btn = new QPushButton("Add");
    QPushButton *clear_btn = new QPushButton("Clear");
    auto pre_group = make_gb("Pre phase:", {"Sequent", "Parallel"});
    auto broad_group = make_gb("Broad phase:", {"Qwattro", "Sort and sweep"});
    auto narrow_group = make_gb("Narrow phase:", {"Sequent", "Parallel"});

    const std::vector<QWidget*> widgets = {
        add_btn,
        clear_btn,
        pre_group.first,
        broad_group.first,
        narrow_group.first
    };

    QVBoxLayout* btn_lo = new QVBoxLayout();
    main_lo->addLayout(btn_lo);

    for (auto& w : widgets) {
        w->setFixedWidth(150);
        btn_lo->addWidget(w);
    }
    btn_lo->addStretch();

    connect(add_btn, &QPushButton::clicked, balls_wgt, &BallsWidget::add_ball);
    connect(clear_btn, &QPushButton::clicked, balls_wgt, &BallsWidget::remove_balls);
    connect(pre_group.second, &QButtonGroup::idClicked, balls_wgt, &BallsWidget::set_pre);
    connect(broad_group.second, &QButtonGroup::idClicked, balls_wgt, &BallsWidget::set_broad);
    connect(narrow_group.second, &QButtonGroup::idClicked, balls_wgt, &BallsWidget::set_narrow);
}

MainWidget::~MainWidget()
{
}

}
