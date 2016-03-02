#include "crestclient.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariantMap>

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
    characterID_ = QString::number(replyObject["CharacterID"].toInt());
    emit characterInfoReceived(
                replyObject["CharacterName"].toString(),
                characterID_
            );
    disconnect(&manager_, SIGNAL(finished(QNetworkReply*)), this, SLOT(onCharacterInfoReply(QNetworkReply*)));
}

void CRESTClient::requestCharacterPortrait(QString characterID)
{
    QNetworkRequest request(QUrl(QString("https://crest-tq.eveonline.com/characters/%1/").arg(characterID)));
    request.setRawHeader(
                "Authorization",
                QString("Bearer %1").arg(QString(accessCode_)).toUtf8()
                );
    manager_.get(request);
    connect(&manager_, SIGNAL(finished(QNetworkReply*)), this, SLOT(onCharacterPortraitReply(QNetworkReply*)));
}

void CRESTClient::onCharacterPortraitReply(QNetworkReply *reply)
{
    auto replyJsonDoc = QJsonDocument::fromJson(reply->readAll());
    auto replyObject = replyJsonDoc.object();
    emit characterPortraitReceived(
                characterID_,
                replyObject["portrait"].toObject()["256x256"].toObject()["href"].toString()
            );
    disconnect(&manager_, SIGNAL(finished(QNetworkReply*)), this, SLOT(onCharacterPortraitReply(QNetworkReply*)));
}

void CRESTClient::requestContactList(QString characterID)
{
    QNetworkRequest request(QUrl(QString("https://crest-tq.eveonline.com/characters/%1/contacts/").arg(characterID)));
    request.setRawHeader(
                "Authorization",
                QString("Bearer %1").arg(QString(accessCode_)).toUtf8()
                );
    auto reply = manager_.get(request);
    connect(reply, SIGNAL(readyRead()), this, SLOT(onContactListReply()));
}

void CRESTClient::onContactListReply()
{
    auto replyJsonDoc = QJsonDocument::fromJson(reinterpret_cast<QNetworkReply*>(sender())->readAll());
    auto replyObject = replyJsonDoc.object();
    auto contacts = replyObject["items"].toArray();

    QVariantList list;
    for (auto it = contacts.begin(); it != contacts.end(); it++) {
        if ((*it).toObject()["contactType"] != "Character") {
            continue;
        }

        QVariantMap map;
        map.insert("name", (*it).toObject()["character"].toObject()["name"].toString());
        map.insert("portrait", (*it).toObject()["character"].toObject()["portrait"].toObject()["256x256"].toObject()["href"].toString());
        map.insert("id_str", (*it).toObject()["character"].toObject()["id_str"].toString());
        map.insert("share", false);
        list.append(map);
    }
    if (characterID_ == "") {
        qDebug("Call requestCharacterInfo first.");
    }
    emit contactListReceived(characterID_, list);
    disconnect(sender(), SIGNAL(readyRead()), this, SLOT(onContactListReply()));
}

void CRESTClient::uploadContacts(QVariantList contacts)
{
    for (auto contact : contacts) {
        QVariantMap rootMap;
        rootMap.insert("standing", 0);
        rootMap.insert("contactType", "Character");
        QVariantMap characterMap;
        characterMap.insert("id_str", contact.toMap()["id_str"].toString());
        characterMap.insert("id", contact.toMap()["id_str"].toString().toInt());
        characterMap.insert("href", QString("https://crest-tq.eveonline.com/characters/%1/").arg(contact.toMap()["id_str"].toString()));
        characterMap.insert("name", contact.toMap()["name"].toString());
        rootMap.insert("contact", characterMap);

        QNetworkRequest request(QUrl(QString("https://crest-tq.eveonline.com/characters/%1/contacts/").arg(characterID_)));
        request.setRawHeader(
                    "Authorization",
                    QString("Bearer %1").arg(QString(accessCode_)).toUtf8()
                    );
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        manager_.post(request, QJsonDocument::fromVariant(QVariant(rootMap)).toJson());
    }
    connect(&manager_, SIGNAL(finished(QNetworkReply*)), this, SLOT(onEndpointsReply(QNetworkReply*)));
}

void CRESTClient::requestEndpoints(QString characterID)
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
