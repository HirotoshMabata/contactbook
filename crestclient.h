#ifndef CRESTCLIENT_H
#define CRESTCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>

class CRESTClient : public QObject
{
    Q_OBJECT
public:
    explicit CRESTClient(QObject *parent = 0);

    Q_PROPERTY(QByteArray accessCode READ accessCode WRITE setAccessCode)
    QByteArray accessCode() { return accessCode_; }
    void setAccessCode(QByteArray accessCode) { accessCode_ = accessCode; }

signals:
    void characterNameReceived(QString characterName);

public slots:
    // return "" if failed to get character name
    void requestCharacterName();
    void onCharacterNameReply(QNetworkReply *reply);

private:
    QNetworkAccessManager manager_;
    QByteArray accessCode_;
};

#endif // CRESTCLIENT_H
