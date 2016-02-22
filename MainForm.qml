import QtQuick 2.5
import QtQuick.Controls 1.4
import QtWebEngine 1.0

Rectangle {
    id: rectangle1
    property QtObject applicationRoot

    Row {
        id: row1
        height: 59
        anchors.top: parent.top
        anchors.topMargin: 8
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8

        Button {
            id: button1
            width: 48
            height: 23
            text: qsTr("Login")
            anchors.verticalCenter: parent.verticalCenter
            onClicked: loginWindowComponent.createObject(applicationRoot)
        }
    }
}
