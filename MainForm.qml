import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import com.hirotosh.crest 1.0

Rectangle {
    id: root
    property QtObject applicationRoot
    property string accessCode
    property Component crestClientComponent: CRESTClient {}

    // ListMode does not support Component for element's property. sync characters and characterList by index.
    property var characters: [{"name": "SHARE", "characterID": "SHARE", "portrait": "", "client": ""}]
    property var pendingClient

    property var contactDatabase: [{"characterID": "SHARE", "contacts": []}]

    function onLogin(code) {
        var client = crestClientComponent.createObject(root)
        client.accessCode = code
        client.requestCharacterInfo()
        pendingClient = client
        client.characterInfoReceived.connect(onCharacterInfoReceived)
    }

    function findIndexOf(array, predicate) {
        for (var i = 0; i < array.length; i++) {
            if (predicate(array[i])) {
                return i
            }
        }
        return -1
    }

    function onCharacterInfoReceived(characterName, characterID) {
        if (characterName === "") {
            loginFailedMessage.open()
            return
        }

        var index = findIndexOf(characters, function(element) {
            return element["name"] === characterName
        })
        if (index < 0) {
            // append new character
            characters.push({
                                "name": characterName,
                                "characterID": characterID,
                                "portrait": "",   // fill it when portrait received
                                "client": pendingClient
                            })
            characterList.append(
                        {
                            "name": characterName,
                            "characterID": characterID,
                            "portrait": "",   // fill it when portrait received
                        })
        } else {
            // refresh client
            characters[index]["client"] = pendingClient
        }

        pendingClient.requestCharacterPortrait(characterID)
        pendingClient.characterPortraitReceived.connect(onCharacterPortraitReceived)

        pendingClient.requestContactList(characterID)
        pendingClient.contactListReceived.connect(onContactListReceived)
    }

    function onCharacterPortraitReceived(characterID, portrait) {
        var index = findIndexOf(characters, function(element) {
            return element["characterID"] === characterID
        })
        characters[index]["portrait"] = portrait
        characterList.setProperty(index, "portrait", portrait)
    }

    function onContactListReceived(characterID, contacts) {
        var tableIndex = findIndexOf(contactDatabase, function(element) {
            return element["characterID"] === characterID
        })
        if (tableIndex < 0) {
            // create new table
            contactDatabase.push(
                        {
                            "characterID": characterID,
                            "contacts": contacts
                        })
        } else {
            // refresh existing table
            contactDatabase[tableIndex]["contacts"] = contacts
        }
        syncContactList(contacts)
    }

    function switchView(characterID) {
        var tableIndex = findIndexOf(contactDatabase, function(element) {
            return element["characterID"] === characterID
        })
        if (tableIndex < 0) {
            return
        }

        syncContactList(contactDatabase[tableIndex]["contacts"])
    }

    // sync contact list UI
    function syncContactList(contacts) {
        contactList.clear()
        for (var i = 0; i < contacts.length; i++) {
            contactList.append(
                        {
                            "name": contacts[i]["name"],
                            "portrait": contacts[i]["portrait"]
                        })
        }
    }

    MessageDialog {
        id: loginFailedMessage
        title: "Failed to login"
        text: "Failed to login EVE server. Please retry later."
    }

    ListView {
        id: characterListView
        height: 80
        anchors.right: buttons.left
        anchors.rightMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 8
        layoutDirection: Qt.RightToLeft
        orientation: ListView.Horizontal
        model: ListModel {
            id: characterList
            ListElement {
                name: "SHARE"
                characterID: "SHARE"
                portrait: "image/74_64_13.png"
            }
        }
        delegate: Item {
            x: 5
            width: 80
            height: 40
            Column {
                spacing: 8

                Image {
                    width: 64
                    height: 64
                    source: portrait
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Text {
                    text: name
                    anchors.horizontalCenter: parent.horizontalCenter
                }

            }
            MouseArea {
                anchors.fill: parent
                onClicked: switchView(id)
            }
        }
    }

    Column {
        id: buttons
        x: 584
        width: 48
        height: 80
        spacing: 4
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 8

        Button {
            width: 48
            height: 80
            id: loginButton
            text: qsTr("Login")
            onClicked: loginWindowComponent.createObject(applicationRoot)
        }
    }

    ListView {
        id: contactListView
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.top: characterListView.bottom
        anchors.topMargin: 8
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
        model: ListModel {
            id: contactList
        }
        delegate: Item {
            x: 5
            width: 80
            height: 40
            Row {
                id: row1
                spacing: 10

                Image {
                    width: 32
                    height: 32
                    source: portrait
                }

                Text {
                    text: name
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }

}
