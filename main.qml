import QtQuick 2.5
import QtQuick.Controls 1.4
import QtWebEngine 1.0

QtObject {
    id: root

    property var mainWindow: ApplicationWindow {
        visible: true

        MainForm {
            applicationRoot: root
            anchors.fill: parent
        }
    }

    property Component loginWindowComponent: LoginWindow {
        visible: true
    }
}
