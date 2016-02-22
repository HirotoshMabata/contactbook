import QtQuick 2.0
import QtQuick.Controls 1.4
import QtWebEngine 1.0

ApplicationWindow {
    property string clientId: "1f337b630dd94813a539a4052805e1a9"
    property string callbackUrl: "https://localhost:6846/auth.callback"

    width: 800
    height: 600
    onClosing: destroy()

    WebEngineView {
        id: loginPage
        url: "https://login.eveonline.com/oauth/authorize/"
                + "?response_type=code"
                + "&redirect_url=" + this.callbackUrl
                + "&client_id=" + this.clientId
                + "&scope=characterContactsRead characterContacsWrite"
        anchors.fill: parent
    }
}
