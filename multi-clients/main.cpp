#include "multiclients.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    multiClients w;
    w.show();
    return a.exec();
}
