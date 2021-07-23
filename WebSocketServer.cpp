#include "WebSocketServer.h"

WebSocketServer::WebSocketServer(int port, QObject *parent) : QObject(parent), webSocketServer(nullptr) {
    this->alarm = new Alarm();
    webSocketServer = new QWebSocketServer("Robot server", QWebSocketServer::SecureMode, this);
    QSslConfiguration sslConfiguration;
    QString certPath = qApp->applicationDirPath().append("/localhost.cert");
    QString keyPath = qApp->applicationDirPath().append("/localhost.key");
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

    qDebug() << "Client connected:" << pSocket->peerName() << pSocket->origin() << pSocket->localAddress();

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

QByteArray WebSocketServer::executeScript(QString script, QString arg) {
    this->process = new QProcess(this->parent());
    QString scriptFile = QDir::currentPath() + QDir::separator() + script + ".py";
    QStringList args = QStringList();
    args << scriptFile;
    args << arg;
    this->process->start("python", args, QIODevice::ReadWrite);
    qDebug() << "EC:" << this->process->exitCode();

    if(!this->process->waitForStarted(-1)){
        qDebug() << "An error occured while starting process";
        return "Error";
    }

    if(this->process->waitForFinished(-1)){
        QByteArray result = this->process->readAllStandardOutput();
        qDebug() << "result" << result;
        this->process->close();
        return result;
    } else {
        qDebug() << "An error occured while running process";
        return "Error";
    }
}

void WebSocketServer::processMessage(QString message) {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << "Received:" << message;
    if(pClient) {
        if(message == "distance"){
            QByteArray data = executeScript(message);
            pClient->sendTextMessage(data);
        } else if (message.contains("lcdprint")){
            QStringList splited = message.split(":");
            QString lcdMessage = splited[1];
            if(lcdMessage.size() > 16){
                pClient->sendTextMessage("Message too long");
                return;
            }
            executeScript("LCD", lcdMessage);
            qDebug() << this->process->readAll();
        } else if (message.contains("wut")) {
            QStringList splited = message.split(" ");
            QString timeString = splited[2].split(".")[0];
#ifdef DEBUG
            qDebug() << timeString;
#endif
            this->alarm->setTime2Buzz(timeString.toStdString().c_str());
            this->alarm->waitForBuzz();
        } else if (message.contains("buzz")) {
            QStringList splited = message.split(":");
            QString cmd = splited[1];
            if(cmd.contains("stop")){
                if(this->alarm->isBuzzing()){
                    this->alarm->stopBuzzing();
                } else {
                    pClient->sendTextMessage("buzzer: false");
                }
            }
        }
    }
}


