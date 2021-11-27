import QtQuick 2.0
import QtQuick.Layouts 1.15
import ".."
import "../Base"
import "../Base/HTile"
import "../Base/Buttons"
import LineEngine 0.1

HTile {
    id: page
    property int taskId


    function onCommandResponse(command, json) {
        if(command === "SearchContactByUserid") {
            if(taskId !== json["task_id"]) {
                return;
            }

            applyButton.loading = false
            var code = json["code"]
            if(code !== 0) {
                error.text = json["content"];
                return
            }
            searchResult.visible = true
            var json_content = JSON.parse(json["content"])
            avatar.mxc = json_content["picturePath"]
            displayName.text = json_content["displayName"]
            statusMessage.text = json_content["statusMessage"]
            avatar.userId = json_content["mid"]
        }
    }
    Component.onDestruction: {
        LineEngine.commandResponse.disconnect(onCommandResponse)
    }

    Component.onCompleted: {
        //  连接回包信号
        LineEngine.commandResponse.connect(onCommandResponse)
    }

    HTabbedBox {
        contentWidth: 500
        contentHeight: 400
        anchors.centerIn: parent
        width: Math.min(implicitWidth, parent.availableWidth)
        height: Math.min(implicitHeight, parent.availableHeight)

        showBackButton: false
        tabBar: HTabBar {
            HTabButton {
                text: "Search Contact"
            }
        }

        HFlickableColumnPage {
            function takeFocus() {

            }
            //输入userid
            HLabeledItem {
                id: userid
                label.text: qsTr("User ID:")
                Layout.fillWidth: true

                HTextField {
                    width: parent.width
                    maximumLength: 255
                    color: utils.nameColor(text)
                }
            }
            //搜索结果
            ContentRow {
                tile: page
                id: searchResult
                visible: false
                //头像
                HUserAvatar {
                    id: avatar
                    mxc: ""
                }
                //名字和签名
                HColumnLayout {
                    Layout.fillWidth: true
                    HRowLayout {
                        spacing: theme.spacing

                        TitleLabel {
                            id : displayName
                            color:  utils.nameColor(text)
                            Behavior on color { HColorAnimation {} }
                        }
                    }

                    SubtitleLabel {
                        id : statusMessage
                        tile: page
                        textFormat: SubtitleLabel.PlainText
                        color: theme.chat.roomPane.listView.member.subtitle
                    }
                }

                //添加按钮
                HButton {
                    text: qsTr("Add Contact")
                    icon.name: "add-chat"
                    onClicked: {

                    }
                }
            }

            HLabel {
                id : error
                color: "red"
                text: ""
                visible: text
            }
        }
        footer: AutoDirectionLayout {
            ApplyButton {
                text: qsTr("Search")
                id: applyButton
                loading: false
                disableWhileLoading: true
                enabled: userid.item.text.trim()
                onClicked: {
                    error.text = ""
                    searchResult.visible = false
                    taskId = LineEngine.searchContact(userid.item.text.trim())
                    loading = true
                }
            }

            CancelButton {
                id: cancelButton
                enabled: !applyButton.loading
                onClicked: {
                    window.mainUI.showPage("", {})
                }
            }
        }
    }
}
