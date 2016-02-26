#include "crestclient.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

CRESTClient::CRESTClient(QObject *parent) : QObject(parent)
{

}

void CRESTClient::requestCharacterInfo()
{
    QNetworkRequest request(QUrl("https://login.eveonline.com/oauth/verify"));
    request.setRawHeader(
                "Authorization",
                QString("Bearer %1").arg(QString(accessCode_)).toUtf8()
                );
    manager_.get(request);
    connect(&manager_, SIGNAL(finished(QNetworkReply*)), this, SLOT(onCharacterInfoReply(QNetworkReply*)));
}

void CRESTClient::onCharacterInfoReply(QNetworkReply *reply)
{
    auto replyJsonDoc = QJsonDocument::fromJson(reply->readAll());
    auto replyObject = replyJsonDoc.object();
    emit characterInfoReceived(
                replyObject["CharacterName"].toString(),
                replyObject["CharacterID"].toInt()
            );
    disconnect(&manager_, SIGNAL(finished(QNetworkReply*)), this, SLOT(onCharacterInfoReply(QNetworkReply*)));
}

void CRESTClient::requestEndpoints(int characterID)
{
    QNetworkRequest request(QUrl(QString("https://crest-tq.eveonline.com/characters/%1/contacts/").arg(characterID)));
    request.setRawHeader(
                "Authorization",
                QString("Bearer %1").arg(QString(accessCode_)).toUtf8()
                );
    manager_.get(request);
    connect(&manager_, SIGNAL(finished(QNetworkReply*)), this, SLOT(onEndpointsReply(QNetworkReply*)));
}

void CRESTClient::onEndpointsReply(QNetworkReply *reply)
{
    qDebug() << reply->readAll();
}
