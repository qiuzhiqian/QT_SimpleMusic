#include "simplemusic.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SimpleMusic w;
    w.show();

    return a.exec();
}
