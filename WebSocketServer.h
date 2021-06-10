#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QtWebSockets/QtWebSockets>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QtNetwork/QSslError>

class WebSocketServer : public QObject {
    Q_OBJECT
public:
    WebSocketServer(int port, QObject *parent = nullptr);

private Q_SLOTS:
    void onNewConnection();
    void processMessage(QString message);
    void socketDisconnected();
    void onSslError(const QList<QSslError> &errors);

private:
    QWebSocketServer *webSocketServer;
    QList<QWebSocket *> clients;
};

#endif // WEBSOCKETSERVER_H
