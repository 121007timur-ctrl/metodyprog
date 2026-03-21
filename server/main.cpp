#include <QCoreApplication>
#include "mytcpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << " Starting Echo Server...";
    MyTcpServer server;

    return a.exec();
}
