#include <QCoreApplication>
#include <iostream>
#include "WebSocketServer.h"
int main(int argc, char *argv[])
{
    std::cout << "WebSocket Hexapod" << std::endl;
    QCoreApplication a(argc, argv);
    WebSocketServer server(3000);
    Q_UNUSED(server);

    return a.exec();
}
