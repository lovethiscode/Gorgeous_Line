import QtQuick 2.15
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.12
import QtQuick.Window 2.15


import "Base"
import "Chat"
import "Session"
import "Pages"
import "Toast"

Item {
    id: mainPane
    Component.onCompleted: window.mainUI = mainPane
    readonly property alias pageLoader: pageLoader
    FontMetrics {
        id: fontMetrics
        font.family: theme.fontFamily.sans
        font.pixelSize: theme.fontSize.normal
        font.pointSize: -1
    }

    LinearGradient {
        id: mainUIGradient
        visible: ! image.visible
        anchors.fill: parent
        start: theme.ui.gradientStart
        end: theme.ui.gradientEnd

        gradient: Gradient {
            GradientStop { position: 0.0; color: theme.ui.gradientStartColor }
            GradientStop { position: 1.0; color: theme.ui.gradientEndColor }
        }
    }
    //设置背景图片
    HImage {
        id: image
        visible: Boolean(Qt.resolvedUrl(source))
        fillMode: Image.PreserveAspectCrop
        animatedFillMode: AnimatedImage.PreserveAspectCrop
        source: theme.ui.image
        sourceSize.width: Screen.width
        sourceSize.height: Screen.height
        anchors.fill: parent
        asynchronous: false
    }

    HRowLayout {
        anchors.fill: parent
        SessionPane {
            width: 200
            Layout.fillHeight: true
            onShowChatPane: {
                chatPane.showChatTab(session)
            }
            onRemoveChatPane: {
                chatPane.removeChatTab(session)
            }
        }

        ChatPane {
            id: chatPane
            Layout.fillHeight: true
            Layout.fillWidth: true

        }
    }

    HLoader {
        source: ""
        id: pageLoader
        anchors.fill: parent
        z: 1001
        anchors.leftMargin:
            mainPane.requireDefaultSize &&
            mainPane.minimumSize > mainPane.maximumSize ?
            mainPane.calculatedSizeNoRequiredMinimum :
            mainPane.visibleSize
    }

    ToastManager {
           id: toast
       }

    function showPage(url, proper) {
        if(url) {
            mask.visible = true
        } else {
            mask.visible = false
        }

        pageLoader.setSource(url, proper)
    }

    function showToast(text, duration) {
        toast.show(text, duration)
    }
    Rectangle {
        id: mask
        anchors.fill: parent
        color: "gray"
        z : 1000
        visible: false
        opacity: 0.5
        MouseArea{
                anchors.fill: parent;
                onClicked: {}
                onReleased: {}
                onPressed: {}
            }
    }
}
