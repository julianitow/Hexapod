#include <QCoreApplication>
#include "WebSocketServer.h"
int main(int argc, char *argv[])
{
    qDebug("Hello World !");
    QCoreApplication a(argc, argv);
    WebSocketServer server(3000);
    Q_UNUSED(server);

    return a.exec();
}
