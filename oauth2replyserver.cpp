#include "oauth2replyserver.h"
#include <QTcpSocket>
#include <QByteArray>

namespace {
    QByteArray prepareReply() {
        QByteArray replyContent = "<html></html>";
        QByteArray reply;
        reply.append("HTTP/1.0 200 OK \r\n");
        reply.append("Content-Type: text/html; charset=\"utf-8\"\r\n");
        reply.append(QString("Content-Length: %1\r\n").arg(replyContent.size()));
        reply.append(replyContent);
        return reply;
    }

    void parseQuery(QByteArray &query) {
        QString firstLine = QString(query).split("\r\n").first();
        firstLine.remove("GET ");
        firstLine.remove("HTTP/1.1");
        firstLine.remove("\r\n");
        firstLine.prepend("http://localhost");
    }
}

OAuth2ReplyServer::OAuth2ReplyServer(QObject *parent) : QTcpServer(parent) {
    connect(this, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    listen(QHostAddress::Any, port_);
}

void OAuth2ReplyServer::setPort(int inPort) {
    port_ = inPort;
    close();
    listen(QHostAddress::Any, port_);
}

void OAuth2ReplyServer::onNewConnection() {
    QTcpSocket *socket = nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()), Qt::UniqueConnection);
    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
}

void OAuth2ReplyServer::onReadyRead() {
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (!socket) {
        return;
    }
    socket->write(prepareReply());
    QByteArray query = socket->readAll();
    socket->disconnectFromHost();
    close();
    emit verificationCodeReceived(QString(query));
}
