import QtQuick 2.12
import QtQuick.Layouts 1.15
import "../Base"
import LineEngine 0.1
import SessionModel 0.1

Rectangle {
    clip: true
    implicitHeight: theme.baseElementsHeight
    color: theme.mainPane.topBar.background

    function onCommandResponse(command, json) {
        if(command === "Disconnect") {
            //显示登录按钮，隐藏profile 按钮
            login.visible = true;
            profile.visible = false;
        }

        if(command === "OnLogin") {
            //登录结果
            if(json["code"] === 0) {
                //隐藏登录按钮，显示配置按钮
                login.visible = false;
                profile.visible = true;
                settings.visible = false
            } else {
                login.visible = true;
                profile.visible = false;
                settings.visible = true
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

        //设置按钮
        HButton {
            id: "settings"
            backgroundColor: "transparent"
            icon.name: "settings"
            onClicked: {
                window.mainUI.showPage("qrc:/gui/Pages/ServerSetting.qml", {})

            }

            Layout.fillHeight: true
        }
        //搜索框

        HTextField {
            id: filterField
            placeholderText: qsTr("Search")
            backgroundColor: theme.mainPane.bottomBar.filterFieldBackground
            bordered: false
            opacity: width >= 16 * theme.uiScale ? 1 : 0

            Layout.fillWidth: true
            Layout.fillHeight: true

            //Keys.forwardTo: [roomList]
            Keys.priority: Keys.AfterItem

            Keys.onTabPressed: {
                console.log("onTabPressed")
            }

            Keys.onBacktabPressed: {
                console.log("onBacktabPressed")
            }

            Keys.onEnterPressed: Keys.onReturnPressed(event)
            Keys.onReturnPressed: {
                console.log("onReturnPressed")
            }

            Keys.onEscapePressed: {
                console.log("onEscapePressed")
            }

            onTextChanged: {
               console.log(filterField.text)
            }

            Behavior on opacity { HNumberAnimation {} }
        }

        HButton {
            id: login
            backgroundColor: "transparent"
            icon.name: "sign-in"
            onClicked: {
                window.mainUI.showPage("qrc:/gui/Pages/AccountLogin.qml", {})
            }
            Layout.fillHeight: true
        }

        HButton {     
            id: profile
            backgroundColor: "transparent"
            icon.name: "username"
            visible: false
            onClicked: {
                window.mainUI.showPage("qrc:/gui/Pages/AccountSettings/AccountSettings.qml", {"profile" : LineEngine.profile})
            }
            Layout.fillHeight: true
        }
    }
}
