#ifndef CRESTCLIENT_H
#define CRESTCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>

class CRESTClient : public QObject
{
    Q_OBJECT
public:
    explicit CRESTClient(QObject *parent = 0);

    Q_PROPERTY(QByteArray accessCode READ accessCode WRITE setAccessCode)
    QByteArray accessCode() { return accessCode_; }
    void setAccessCode(QByteArray accessCode) { accessCode_ = accessCode; }

signals:
    void characterInfoReceived(QString characterName, int characterID);
    void characterPortraitReceived(int characterID, QString portraitUrl);
    void contactListReceived(QVariantList contacts);

public slots:
    // return "" if failed to get character name
    void requestCharacterInfo();
    void requestContactList(int characterID);
    void requestCharacterPortrait(int characterID);

    void onCharacterInfoReply(QNetworkReply *reply);
    void onContactListReply(QNetworkReply *reply);
    void onCharacterPortraitReply(QNetworkReply *reply);

    void requestEndpoints(int characterID);
    void onEndpointsReply(QNetworkReply *reply);

private:
    QNetworkAccessManager manager_;
    QByteArray accessCode_;
};

#endif // CRESTCLIENT_H
