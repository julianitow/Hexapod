#include "WebSocketServer.h"

WebSocketServer::WebSocketServer(int port, QObject *parent) : QObject(parent), webSocketServer(nullptr) {
    webSocketServer = new QWebSocketServer("Robot server", QWebSocketServer::SecureMode, this);
    QSslConfiguration sslConfiguration;
    QString certPath = qApp->applicationDirPath().append("./localhost.cert");
    QString keyPath = qApp->applicationDirPath().append("./localhost.key");
    QFile certFile(certPath);
    QFile keyFile(keyPath);
    certFile.open(QIODevice::ReadOnly);
    keyFile.open(QIODevice::ReadOnly);
    if(certFile.isOpen() && keyFile.isOpen()){
        QSslCertificate certificate(&certFile, QSsl::Pem);
        QSslKey sslKey(&keyFile, QSsl::Rsa, QSsl::Pem);
        certFile.close();
        keyFile.close();
        sslConfiguration.setPeerVerifyMode(QSslSocket::VerifyNone);
        sslConfiguration.setLocalCertificate(certificate);
        sslConfiguration.setPrivateKey(sslKey);
        sslConfiguration.setProtocol(QSsl::TlsV1SslV3);
        webSocketServer->setSslConfiguration(sslConfiguration);

        if (webSocketServer->listen(QHostAddress::Any, port))
        {
            qDebug() << "SSL Echo Server listening on port" << port;
            connect(webSocketServer, &QWebSocketServer::newConnection,
                    this, &WebSocketServer::onNewConnection);
            connect(webSocketServer, &QWebSocketServer::sslErrors,
                    this, &WebSocketServer::onSslError);
        }
    } else {
        qDebug() << "Error opening files";
    }
}

void WebSocketServer::onNewConnection()
{
    QWebSocket *pSocket = webSocketServer->nextPendingConnection();

    qDebug() << "Client connected:" << pSocket->peerName() << pSocket->origin();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &WebSocketServer::processMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &WebSocketServer::socketDisconnected);

    clients << pSocket;
}

void WebSocketServer::socketDisconnected()
{
    qDebug() << "Client disconnected";
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient)
    {
        clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

void WebSocketServer::onSslError(const QList<QSslError> &)
{
    qDebug() << "Ssl errors occurred";
}

void WebSocketServer::processMessage(QString message) {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << "Received:" << message;
    if(pClient) {
        pClient->sendTextMessage("Well received: " + message);
    }
}


