#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <iostream>
#include <QtWebSockets/QtWebSockets>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QtNetwork/QSslError>
#include "alarm.h"

#include "alarm.h"

class WebSocketServer : public QObject {
    Q_OBJECT
public:
    WebSocketServer(int port, QObject *parent = nullptr);
    static void threadExec(WebSocketServer*, std::string, std::string = "");
    void stopPod();
public slots:
    QByteArray executeScript(WebSocketServer*, QString script, QString arg = "");
    QString alarmStatus();

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
    QProcess* stopPodProcess;
    Alarm* alarm;
    static std::thread* movementThread;
    static pthread_t movementHandle;
};

#endif // WEBSOCKETSERVER_H
