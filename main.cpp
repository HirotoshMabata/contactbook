#include <QGuiApplication>
#include <QtQml>
#include <QQmlApplicationEngine>
#include <QtWebEngine/qtwebengineglobal.h>
#include "oauth2replyserver.h"
#include "oauth2accesstokenclient.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QtWebEngine::initialize();

    qmlRegisterType<OAuth2ReplyServer>("com.hirotosh.oauth2", 1, 0, "OAuth2ReplyServer");
    qmlRegisterType<OAuth2AccessTokenClient>("com.hirotosh.oauth2", 1, 0, "OAuth2AccessTokenClient");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
