#include "myserver.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyServer server(2323);
    server.showMaximized();
    return a.exec();
}
