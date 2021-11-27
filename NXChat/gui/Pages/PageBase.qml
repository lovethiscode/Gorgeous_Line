import QtQuick 2.0
import QtQuick.Layouts 1.15
import "../Base"
import "../Base/Buttons"
Item {
    id: page
    property string title
    property int fixWidth
    property int fixHeight
    property Component content
    property alias applyBtnVisible: applyButton.visible
    property alias applyBtnEnabled: applyButton.enabled
    property alias applyBtnText: applyButton.text
    property alias cancelBtnText: cancelButton.text
    property alias applyBtnIcon: applyButton.icon.name
    property alias cancelBtnIcon: cancelButton.icon.name
    property alias customMargin: flickPage.customMargin

    signal applyBtnClicked()
    signal cancelBtnClicked()

    anchors.fill: parent
    HTabbedBox {
        anchors.centerIn: parent
        width:  fixWidth || Math.min(implicitWidth, parent.availableWidth)
        height: fixHeight || Math.min(implicitHeight, parent.availableHeight)

        showBackButton: false
        tabBar: HTabBar {
            HTabButton {
                text: page.title
            }
        }
        HFlickableColumnPage {
            id : flickPage
            function takeFocus() {

            }

            HLoader {
                asynchronous: false
                sourceComponent: content

                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            footer: AutoDirectionLayout {
                ApplyButton {
                    id: applyButton
                    loading: false
                    disableWhileLoading: false
                    onClicked: {
                        applyBtnClicked()
                    }
                }

                CancelButton {
                    id: cancelButton
                    onClicked: {
                        cancelBtnClicked()
                    }
                }
            }
        }
    }
}
