import QtQuick 2.5
import QtQuick.Controls 1.4
import QtWebEngine 1.0

QtObject {
    id: root

    property var mainWindow: ApplicationWindow {
        width: 800
        height: 600
        id: main
        visible: true

        MainForm {
            id: mainForm
            applicationRoot: root
            anchors.fill: parent
        }
    }

    property Component loginWindowComponent: LoginWindow {
        id: loginWindow
        visible: true
        onAccessCodeReceived: mainForm.onLogin(code)
    }
}
