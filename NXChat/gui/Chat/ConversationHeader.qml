import QtQuick 2.15
import QtQuick.Layouts 1.15
import "../Base"
import "../Base/HTile"

Rectangle {
    id: root
    property QtObject session;
    implicitHeight: 58
    color: "steelblue"
    HRowLayout {
        id: row
        anchors.fill: parent

        //名字和状态
        HColumnLayout {
            id: title
            HLabel {
                id: displayname
                text: session.display_name
                font.pixelSize: theme.fontSize.bigger
                color: theme.chat.roomHeader.name
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
                leftPadding: theme.spacing
                rightPadding: leftPadding

                // FIXME: these dirty manual calculations
                Layout.preferredWidth: Math.min(
                    implicitWidth,
                    row.width
                )
                Layout.fillWidth: ! topicLabel.text
                Layout.fillHeight: true

                HoverHandler { id: nameHover }
            }

            HLabel {
                id: topicLabel
                text: session.status_msg || ""
                textFormat: Text.StyledText
                leftPadding: theme.spacing
                font.pixelSize: theme.fontSize.big
                color: theme.chat.roomHeader.topic

                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter

                Layout.preferredWidth: ! text ? 0 : Math.min(
                    implicitWidth,
                    row.width
                )

                Layout.fillWidth: text && ! root.center
                Layout.fillHeight: true
            }
        }

        //顶部右边的操作栏
        ChatHeaderButton {
            id: copyButton
            icon.name: "room-header-copy"
            backgroundColor: "transparent"

            onClicked: {
               console.log("Copy")
            }
        }

        ChatHeaderButton {
            id: deselectButton
            backgroundColor: "transparent"
            icon.name: "room-header-deselect"
            onClicked: {
                window.makePopup(
                                "Popup/PopupTest.qml",
                                {
                                    "anchors.fill": "parent"
                                },
                                obj => {
                                },
                            )
            }
        }
        //占位
        Item  {
            width: 10
        }
    }
}
