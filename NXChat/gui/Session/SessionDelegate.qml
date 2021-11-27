import QtQuick 2.12
import QtQuick.Layouts 1.15
import "../Base"
import "../Base/HTile"

HTile {
    id: account
    backgroundColor: theme.mainPane.listView.account.background
    contentItem: ContentRow {
        tile: account
        spacing: 0
        //头像
        HUserAvatar {
            id: avatar
            clientUserId: model.id
            userId: model.id
            displayName: model.display_name
            mxc: model.avatar_url
            radius: theme.mainPane.listView.account.avatarRadius
            compact: account.compact
            presence: model.presence
            Layout.alignment: Qt.AlignLeft
        }
        //名字和状态
        HColumnLayout {
            id: title

            TitleLabel {
                text: model.display_name || model.id
                font.pixelSize: 16
                color:
                    hovered && model.id ?
                    utils.nameColor(
                        model.display_name || model.id.substring(0, 1),
                    ) :
                    theme.mainPane.listView.account.name

                Behavior on color { HColorAnimation {} }

                Layout.leftMargin: theme.spacing
            }

            SubtitleLabel {
                id: statusMsg
                tile: account
                textFormat: SubtitleLabel.PlainText
                font.pixelSize: 12
                text: model.status_msg.trim()
                visible: Boolean(text)             
                Layout.leftMargin: theme.spacing
            }
            HoverHandler { id: nameHover }

              HToolTip {
                  visible: nameHover.hovered && statusMsg.text
                  text: statusMsg.text
              }
        }
        //消息未读数
        HLabel {
            z: 1
            visible: sessionList.currentIndex === model.index ? false :
                                                                model.total_unread ? true : false

            text: "(" + model.total_unread + ")"
            font.pixelSize: parent.height / 3

            color: utils.hsluv(
               text ? utils.hueFrom(text) : 0,
               text ? theme.controls.avatar.letter.saturation : 0,
               theme.controls.avatar.letter.lightness,
               theme.controls.avatar.letter.opacity
           )
        }
    }

    function openContextMenu() {
        if(sessionList.currentIndex == -1) {
            return
        }

        sessionList.contextMenu.spawn(model, sessionList.currentIndex)
    }

    DelegateTransitionFixer {}
    TapHandler {
        acceptedButtons: Qt.RightButton
        acceptedPointerTypes: PointerDevice.GenericPointer | PointerDevice.Pen
        onTapped: openContextMenu()
    }
}
