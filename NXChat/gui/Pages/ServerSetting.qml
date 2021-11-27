import QtQuick 2.0
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import "../Base"
import "../Base/Buttons"
import GlobalConfig 0.1
import LineEngine 0.1

Item {
    id: page

    property int type: 1

    function closePage() {
         window.mainUI.showPage("", {})
     }

    HTabbedBox {
        anchors.centerIn: parent
        width: Math.min(implicitWidth, parent.availableWidth)
        height: Math.min(implicitHeight, parent.availableHeight)


        showBackButton: false
        tabBar: HTabBar {
            HTabButton {
                text: "Server Setting"
            }
            HTabButton {
                text: "Proxy Setting"
            }
        }

        HFlickableColumnPage {
            function takeFocus() {
                //服务器设置
                type = 1;
            }

            HLabeledItem {
               id: serverField
               label.text: qsTr("Server:")

               Layout.fillWidth: true
               HTextField {
                   width: parent.width
                   text:  GlobalConfig.serverInfo.server
               }
           }

           HLabeledItem {
               id: portField
               label.text: qsTr("Port:")
               Layout.fillWidth: true

               HTextField {
                   width: parent.width
                   text:  GlobalConfig.serverInfo.port.toString()
                   validator : IntValidator {
                       bottom: 1
                       top:65535
                   }
               }
           }
        }

        HFlickableColumnPage {
            function takeFocus() {
                //代理设置
                type = 2;
            }

            HLabeledItem {
                label.text: qsTr("Proxy Type:")
                Layout.fillWidth: true
                HComboBox {
                    id: proxyTypeCombox
                    editable : false
                    width: parent.width
                    model: ["SOCKS5", "HTTP"]
                    currentIndex: GlobalConfig.proxyInfo.proxyType || 0
                    onCurrentIndexChanged: {
                        if(currentIndex === -1) {
                            return
                        } else if (currentIndex === 0) {
                            proxyUserNameField.visible = false;
                            proxyPasswordField.visible = false;
                        } else if (currentIndex === 1) {
                            proxyUserNameField.visible = true;
                            proxyPasswordField.visible = true;
                        }
                      }
                   }
               }

            HLabeledItem {
                id: proxyServerField
                label.text: qsTr("Proxy Server:")
                Layout.fillWidth: true

                HTextField {
                    width: parent.width
                    text: GlobalConfig.proxyInfo.proxyServer || ""
                }
            }

            HLabeledItem {
                id: proxyPortField
                label.text: qsTr("Proxy Port:")
                Layout.fillWidth: true

                HTextField {
                    width: parent.width
                    validator : IntValidator {
                        bottom: 1
                        top:65535
                    }
                    text: GlobalConfig.proxyInfo.proxyPort || 1080
                }
            }

            HLabeledItem {
                id: proxyUserNameField
                label.text: qsTr("Proxy UserName(Optional):")
                Layout.fillWidth: true

                HTextField {
                    text: GlobalConfig.proxyInfo.proxyUsername || ""
                    width: parent.width
                }
            }

            HLabeledItem {
                id: proxyPasswordField
                label.text: qsTr("Proxy Password(Optional):")
                Layout.fillWidth: true

                HTextField {
                    text: GlobalConfig.proxyInfo.proxyPassword || ""
                    width: parent.width
                }
            }
            HCheckBox {
                id: enableProxy
                text: qsTr("Enable Proxy")
                defaultChecked: GlobalConfig.proxyInfo.enableProxy
                Layout.fillWidth: true
            }
        }

        footer: AutoDirectionLayout {
            ApplyButton {
                text: qsTr("OK")
                id: applyButton
                loading: false
                disableWhileLoading: false
                enabled: type === 1 ? serverField.item.text.trim() && portField.item.text.trim()
                                   : proxyTypeCombox.editText.trim() && proxyServerField.item.text.trim() && proxyPortField.item.text.trim()
                onClicked: {
                    if(type === 1) {
                        //设置服务器
                        var serverInfo = {}
                        serverInfo.server = serverField.item.text.trim()
                        serverInfo.port = parseInt(portField.item.text.trim())
                        GlobalConfig.serverInfo = serverInfo
                        closePage()
                        window.mainUI.showToast(qsTr("Set server complete"), 2000)
                    } else if (type === 2) {
                        var proxyInfo = {}
                        proxyInfo.proxyType = proxyTypeCombox.currentIndex;
                        proxyInfo.proxyServer = proxyServerField.item.text;
                        proxyInfo.proxyPort = parseInt(proxyPortField.item.text)
                        proxyInfo.proxyUsername = proxyUserNameField.item.text;
                        proxyInfo.proxyPassword = proxyPasswordField.item.text
                        proxyInfo.enableProxy = enableProxy.checked


                        GlobalConfig.proxyInfo = proxyInfo
                        closePage()
                        window.mainUI.showToast(qsTr("Set proxy complete"), 2000)
                    }
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
