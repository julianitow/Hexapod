#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <iostream>
#include <QtWebSockets/QtWebSockets>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QtNetwork/QSslError>

#include "Alarm.h"

class WebSocketServer : public QObject {
    Q_OBJECT
public:
    WebSocketServer(int port, QObject *parent = nullptr);
public slots:
    QByteArray executeScript(QString script, QString arg = "");

signals:
    void commandReceived(QString command);

private Q_SLOTS:
    void onNewConnection();
    void processMessage(QString message);
    void socketDisconnected();
    void onSslError(const QList<QSslError> &errors);

private:
    QWebSocketServer *webSocketServer;
    QList<QWebSocket *> clients;
    QProcess* process;
};

#endif // WEBSOCKETSERVER_H
