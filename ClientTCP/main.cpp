#include "clienttcp.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ClientTCP w;
    w.show();
    return a.exec();
}
