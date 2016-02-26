TEMPLATE = app

QT += core qml quick webengine network
CONFIG += c++11

SOURCES += main.cpp \
    oauth2replyserver.cpp \
    crestclient.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    oauth2replyserver.h \
    crestclient.h
