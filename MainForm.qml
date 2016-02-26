import QtQuick 2.5
import QtQuick.Controls 1.4
import com.hirotosh.crest 1.0

Rectangle {
    id: root
    property QtObject applicationRoot
    property string accessCode
    property Component crestClientComponent: CRESTClient {}

    // sync characterNames, client, characterList by index
    property var clients: []
    property var characterNames: []

    signal login (string code)
    onLogin: {
        var client = crestClientComponent.createObject(root)
        var characterName = client.getCharacterName(code)
        var index = characterNames.indexOf(characterName)
        if (index < 0) {
            // append new character
            characterNames.push(characterName)
            clients.push(client)
            characterList.append({"name": characterName})
        } else {
            // refresh client
            clients[index] = client
        }
    }

    Text {
        id: accessCodeViewer
        x: 16
        text: accessCode
        anchors.top: characterListView.bottom
        anchors.topMargin: 8
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
        font.pixelSize: 12
    }

    Button {
        id: loginButton
        x: 584
        width: 48
        height: 80
        text: qsTr("Login")
        anchors.top: parent.top
        anchors.topMargin: 8
        anchors.right: parent.right
        anchors.rightMargin: 8
        onClicked: loginWindowComponent.createObject(applicationRoot)
    }

    ListView {
        id: characterListView
        height: 80
        anchors.right: loginButton.left
        anchors.rightMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 8
        layoutDirection: Qt.RightToLeft
        orientation: ListView.Horizontal
        model: ListModel {
            id: characterList
        }
        delegate: Item {
            x: 5
            width: 80
            height: 40
            Row {
                id: row2
                spacing: 10

                Text {
                    text: name
                    anchors.verticalCenter: parent.verticalCenter
                    font.bold: true
                }
            }
        }
    }
}
