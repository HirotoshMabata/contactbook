#ifndef OAUTH2REPLYSERVER_H
#define OAUTH2REPLYSERVER_H

#include <QTcpServer>

class OAuth2ReplyServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit OAuth2ReplyServer(QObject *parent = 0);

    Q_PROPERTY(int port READ port WRITE setPort)
    int port() { return port_; }
    void setPort(int inPort);

signals:
    void accessCodeReceived(QString code);

public slots:
    void onNewConnection();
    void onReadyRead();

private:
    int port_;
};

#endif // OAUTH2REPLYSERVER_H
