import QtQuick 2.12
import QtQuick.Layouts 1.15
import "../Base"
import LineEngine 0.1

Rectangle {
    clip: true
    implicitHeight: theme.baseElementsHeight
    color: "steelblue"

    function onCommandResponse(command, json) {
        if(command === "Disconnect") {
            //显示登录按钮，隐藏profile 按钮
            addChat.visible = false
        }

        if(command === "OnLogin") {
            //登录结果
            if(json["code"] === 0) {
                //隐藏登录按钮，显示配置按钮
                addChat.visible = true
            } else {
                addChat.visible = false
            }
        } else if(command === "OnNotify") {
            const subCommand = json["subcommand"]
            if(subCommand === "NOTIFIED_INVITE_INTO_CHAT") {
                chatNotify.icon.name = "notifications-highlights-only"
            }
        }
    }

    Component.onDestruction: {
        LineEngine.commandResponse.disconnect(onCommandResponse)
    }

    Component.onCompleted: {
        //  连接回包信号
        LineEngine.commandResponse.connect(onCommandResponse)
    }

    HRowLayout {
        anchors.fill: parent
        HButton {
            id: chatNotify
            backgroundColor: "transparent"
            icon.name: "notifications-enable"
            visible: false
            onClicked: {
               window.mainUI.showPage("qrc:/gui/Pages/GroupInvite/GroupInvite.qml", {})
               icon.name = "notifications-enable"
            }
            Layout.alignment: Qt.AlignLeft
            Layout.fillHeight: true
        }


        HButton {
            id: addChat
            backgroundColor: "transparent"
            icon.name: "menu-add-chat"
            visible: false
            onClicked: {
               window.mainUI.showPage("qrc:/gui/Pages/CreateSession.qml", {})
            }
            Layout.alignment: Qt.AlignRight
            Layout.fillHeight: true
        }
    }
}
