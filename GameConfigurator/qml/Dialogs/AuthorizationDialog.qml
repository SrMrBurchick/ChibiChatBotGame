import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Panels
import Managers
import QtWebEngine

Dialog {
    id: root
    width: 350
    height: 550

    anchors.centerIn: parent

    title: "Twitch Authorization"
    standardButtons: Dialog.Cancel | Dialog.Apply
    WebEngineView {
        id: webEngine
        anchors.fill: parent
        onUrlChanged: {
            console.log(url)
            if (url.toString().startsWith("https://localhost:1337")) {
                var tokenIndex = url.toString().indexOf("access_token=");
                if (tokenIndex !== -1) {
                    var tokenEndIndex = url.toString().indexOf("&", tokenIndex);
                    if (tokenEndIndex === -1) {
                        tokenEndIndex = url.toString().length;
                    }
                    var token = url.toString().substring(tokenIndex + 13, tokenEndIndex);
                    console.log("Token:", token);
                    TwitchManager.userAuthorized(token)
                    root.close()
                }
            }
        }
    }

    function setup(url) {
        console.log(url)
        webEngine.url = url
    }
}
