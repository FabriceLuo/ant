#include "Ant.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Ant w;
    w.show();

    return a.exec();
}
