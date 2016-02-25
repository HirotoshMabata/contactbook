#include "oauth2replyserver.h"
#include <QTcpSocket>
#include <QByteArray>

namespace {
    QByteArray prepareReply() {
        QByteArray replyContent =
                "<!DOCTYPE html>"
                "<html>"
                "<body>"
                "DONE."
                "<script>"
                "if (window.location.hash.length > 0) {"
                "  window.location = window.location.origin.concat('/auth.callback?', window.location.hash.substr(1))"
                "}"
                "</script>"
                "</body>"
                "</html>";
        QByteArray reply;
        reply.append("HTTP/1.1 200 OK \r\n");
        reply.append("Content-Type: text/html; charset=\"utf-8\"\r\n");
        reply.append(QString("Content-Length: %1\r\n\r\n").arg(replyContent.size()));
        reply.append(replyContent);
        return reply;
    }

    QString parseQuery(QByteArray &query) {
        QString firstLine = QString(query).split("\r\n").first();
        firstLine.remove("GET ");
        firstLine.remove(" HTTP/1.1");
        firstLine.remove("\r\n");
        firstLine.remove("/auth.callback?access_token=");       // TODO: parse using redirect_uri
        firstLine.remove("&token_type=Bearer&expires_in=1200"); // TODO: parse argument-wise

        return firstLine;
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
    if (query.contains("access_token")) {
        close();
        emit accessCodeReceived(parseQuery(query));
    }
}
