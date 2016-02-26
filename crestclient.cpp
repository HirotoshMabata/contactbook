#include "crestclient.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

CRESTClient::CRESTClient(QObject *parent) : QObject(parent)
{

}

void CRESTClient::requestCharacterName()
{
    QNetworkRequest request(QUrl("https://login.eveonline.com/oauth/verify"));
    request.setRawHeader(
                "Authorization",
                QString("Bearer %1").arg(QString(accessCode_)).toUtf8()
                );
    manager_.get(request);
    connect(&manager_, SIGNAL(finished(QNetworkReply*)), this, SLOT(onCharacterNameReply(QNetworkReply*)));
}

void CRESTClient::onCharacterNameReply(QNetworkReply *reply)
{
    auto replyJsonDoc = QJsonDocument::fromJson(reply->readAll());
    emit characterNameReceived(replyJsonDoc.object()["CharacterName"].toString());
}
