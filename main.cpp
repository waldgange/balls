#include "mainwidget.h"

#include <QApplication>
#include <QLoggingCategory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QLoggingCategory::defaultCategory()->setEnabled(QtDebugMsg, true);
    Balls::MainWidget w;
    w.show();
    return a.exec();
}
