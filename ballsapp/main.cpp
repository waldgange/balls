#include "mainwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Balls::MainWidget w;
    w.show();
    return a.exec();
}
