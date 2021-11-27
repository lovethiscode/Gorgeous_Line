import QtQuick 2.0
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import "../Base"
import "../Base/Buttons"
import GlobalConfig 0.1
import LineEngine 0.1

Item {
    id: page
    property var accounts: GlobalConfig.getAccounts()

    property int type: 0
    function onCommandResponse(command, json) {
        if(command === "Disconnect") {
            window.mainUI.showToast(qsTr("Disconnect from server"), 5000)
            applyButton.loading = false
        } else if(command === "OnLogin") {
            applyButton.loading = false
            //登录结果
            if(json["code"] === 0) {
                window.mainUI.showToast(qsTr("Login success"), 2000)
                window.mainUI.showPage("", {})
            } else {
                window.mainUI.showToast(json["content"], 5000)
            }
        }
    }
    Component.onDestruction: {
        LineEngine.commandResponse.disconnect(onCommandResponse)
    }

    Component.onCompleted: {
        //  连接回包信号
        LineEngine.commandResponse.connect(onCommandResponse)

        var emailModel = []
        var phoneModel = []
        for(var key in accounts) {
            var account = accounts[key]
            if(account.type === 0) {
                emailModel[emailModel.length] = key
            } else if(account.type === 1) {
                phoneModel[phoneModel.length] = key
            }
        }

        phoneCombox.model = phoneModel
        userNameCombo.model = emailModel
    }

    HTabbedBox {
        anchors.centerIn: parent
        width: Math.min(implicitWidth, parent.availableWidth)
        height: Math.min(implicitHeight, parent.availableHeight)


        showBackButton: false
        tabBar: HTabBar {
            HTabButton {
                text: "Email"
            }
            HTabButton {
                text: "Phone"
            }
        }

        HFlickableColumnPage {
            function takeFocus() {
                //email 登录
                type = 0;
            }

            HLabeledItem {
                id: usernameField
                label.text: qsTr("Username:")
                HComboBox {
                    id: userNameCombo
                    editable : true
                    width: parent.width
                    model: []

                    onCurrentIndexChanged: {
                        if(currentIndex === -1) {
                            passwordField.item.text = ""
                            certField.item.text = ""
                            return
                        }
                        passwordField.item.text = accounts[model[currentIndex]].password
                        certField.item.text = accounts[model[currentIndex]].token
                      }
                   }
                }         
                HLabeledItem {
                    id: passwordField
                    label.text: qsTr("Password:")
                    Layout.fillWidth: true

                    HTextField {
                        width: parent.width
                    }
                }

                HLabeledItem {
                    id: certField
                    label.text: qsTr("Cert:")
                    Layout.fillWidth: true

                    HTextField {
                        width: parent.width
                    }
             }           
        }

        HFlickableColumnPage {
            function takeFocus() {               
                //手机号登录
                type = 1;
            }
            HLabeledItem {
                id: phoneField
                    label.text: qsTr("Phone:")

                    Layout.fillWidth: true
                    HComboBox {
                        id: phoneCombox
                        editable : true
                        width: parent.width
                        model: []

                        onCurrentIndexChanged: {
                            if(currentIndex === -1) {
                                tokenField.item.text = ""
                                return
                            }

                            tokenField.item.text = accounts[model[currentIndex]].token
                          }
                       }
                }

            HLabeledItem {
                id: tokenField
                label.text: qsTr("AuthToken:")
                Layout.fillWidth: true

                HTextField {
                    width: parent.width
                }
            }
        }

        footer: AutoDirectionLayout {
            ApplyButton { 
                text: qsTr("Login")
                id: applyButton
                loading: false
                disableWhileLoading: false
                enabled: type === 0 ? userNameCombo.editText.trim() && passwordField.item.text.trim() && certField.item.text.trim()
                                   : phoneCombox.editText.trim() && tokenField.item.text.trim()
                onClicked: {
                    loading = true
                    var params = {}
                    params["type"] = type
                    if(type === 0) {
                        //emila 登录
                        params["user"] = userNameCombo.editText.trim()
                        params["password"] = passwordField.item.text.trim()
                        params["token"] = certField.item.text.trim()
                    } else if (type === 1) {
                        params["user"] = phoneCombox.editText.trim()
                        params["token"] = tokenField.item.text.trim()
                    }

                    LineEngine.login(params)
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
