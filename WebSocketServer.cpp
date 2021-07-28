#include "WebSocketServer.h"
pthread_t WebSocketServer::movementHandle = NULL;
std::thread* WebSocketServer::movementThread = nullptr;

WebSocketServer::WebSocketServer(int port, QObject *parent) : QObject(parent), webSocketServer(nullptr) {
    this->alarm = Alarm::getAlarm();
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

void WebSocketServer::threadExec(WebSocketServer* server, std::string script, std::string arg) {
    qDebug() << server->executeScript(server, QString::fromStdString(script), QString::fromStdString(arg));
}

QByteArray WebSocketServer::executeScript(WebSocketServer* server, QString script, QString arg) {
    server->process = new QProcess(nullptr);
    QString scriptFile = QDir::currentPath() + QDir::separator() + script + ".py";
    QStringList args = QStringList();
    args << scriptFile;
    args << arg;
    server->process->setProgram("python");
    server->process->setArguments(args);
    //this->process->startDetached();
    server->process->open(QIODevice::ReadWrite);

    if(!server->process->waitForStarted(-1)){
        qDebug() << "An error occured while starting process";
        return "Error";
    }

    if(this->process->waitForFinished(-1)){
        QByteArray result = server->process->readAllStandardOutput();
        qDebug() << "result" << result;
        qDebug() << "Output:" << server->process->readAllStandardOutput();
        qDebug() << "Error:" << server->process->readAllStandardError();
        qDebug() << "EC:" << server->process->exitCode();
        server->process->close();
        return result;
    } else {
        qDebug() << "An error occured while running process";
        return "Error";
    }
}

QString WebSocketServer::alarmStatus(){
    if(Alarm::isAlarmExists()){
        std::cout << "HEEERE" << std::endl;
        return "alarm:" + QString(Alarm::getAlarm()->getTime2Buzz());
    }
    std::cout << "Et bah non chech !" << std::endl;
    return "";
}

void WebSocketServer::stopPod(){
    this->stopPodProcess = new QProcess(nullptr);
    QStringList args = QStringList();
    args << QDir::currentPath() + QDir::separator() + "stopPod.py";
    this->stopPodProcess->setProgram("python");
    this->stopPodProcess->setArguments(args);
    this->stopPodProcess->open(QIODevice::ReadWrite);
}

void WebSocketServer::processMessage(QString message) {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << "Received:" << message;
    if(pClient) {
        if(message == "forward" || message == "back"){
            this->movementThread = new std::thread(WebSocketServer::threadExec, this, message.toStdString(), "");
            WebSocketServer::movementHandle = this->movementThread->native_handle();
            this->movementThread->detach();
            //QByteArray data = executeScript(message);
            pClient->sendTextMessage("OK");
        } else if (message == "stop") {
            pthread_cancel(WebSocketServer::movementHandle);
            if(this->process != nullptr){
                if(this->process->isOpen()){
                    this->process->close();
                    this->stopPod();
                }
            }
        }else if (message.contains("lcdprint")){
            QStringList splited = message.split(":");
            QString lcdMessage = splited[1];
            if(lcdMessage.size() > 16){
                pClient->sendTextMessage("Message too long");
                return;
            }
            executeScript(this, "LCD", lcdMessage);
            qDebug() << this->process->readAll();
        } else if (message.contains("wut")) {
            QStringList splited = message.split(" ");
            QString timeString = splited[2].split(".")[0];
            Alarm::getAlarm()->setTime2Buzz(timeString.toStdString().c_str());
            Alarm::getAlarm()->waitForBuzz(timeString.toStdString().c_str());
        } else if (message.contains("buzz")) {
            QStringList splited = message.split(":");
            QString cmd = splited[1];
            if(cmd.contains("stop")){
                Alarm::getAlarm()->stopBuzzing();
            }
        } else if (message.contains("alarmState")){
            if(!Alarm::isAlarmExists()){
                pClient->sendTextMessage("No active alarm set");
                std::cout << "Sent: No active nia nia " << std::endl;
                return;
            }
            QString status = "status: " + QString::fromStdString(Alarm::getAlarm()->status(pClient));
            //std::cout << "AlarmState: " << status.toStdString() << std::endl;
            //pClient->sendTextMessage(status);
        }
    }
}


