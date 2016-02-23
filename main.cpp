#include <QGuiApplication>
#include <QtQml>
#include <QQmlApplicationEngine>
#include <QtWebEngine/qtwebengineglobal.h>
#include "oauth2replyserver.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QtWebEngine::initialize();

    qmlRegisterType<OAuth2ReplyServer>("com.hirotosh.oauth2", 1, 0, "OAuth2ReplyServer");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
