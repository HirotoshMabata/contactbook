import QtQuick 2.0
import QtQuick.Controls 1.4
import QtWebEngine 1.0
import com.hirotosh.oauth2 1.0

ApplicationWindow {
    property string clientId: "1f337b630dd94813a539a4052805e1a9"
    property string redirectUri: "http://localhost:6846/auth.callback"

    width: 800
    height: 600
    onClosing: destroy()


    WebEngineView {
        id: loginPage
        anchors.fill: parent
        anchors.bottomMargin: 300
        url: "https://login.eveonline.com/oauth/authorize/"
              + "?response_type=token"
              + "&redirect_uri=" + redirectUri
              + "&client_id=" + clientId
              + "&scope=characterContactsRead"
    }

    OAuth2ReplyServer {
        id: server
        port: 6846
    }

    OAuth2AccessTokenClient {
        id: accessTokenClient
    }
}
