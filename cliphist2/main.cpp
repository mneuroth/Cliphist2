#include <QtGui/QApplication>
#include "cliphistwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CliphistWindow w;
    w.show();
    return a.exec();
}
