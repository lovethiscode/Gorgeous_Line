import QtQuick 2.0
import QtQuick.Layouts 1.15
import "../Base"
import "../Base/HTile"

import LineEngine 0.1

Rectangle {
    id: root
    color: theme.controls.button.background
    anchors.fill: parent

    readonly property alias contextMenu: contextMenu
    //点击联系人信号
    signal clickSession(QtObject session)
    signal removeSession(QtObject session)

    Component {
         id: sectionDelegate
         TitleLabel {
             font.pixelSize: theme.fontSize.bigger
             width: ListView.view.width
             height: 20
             text: section.substring(0, 1)
             color: theme.mainPane.listView.account.name
             horizontalAlignment: Qt.AlignHCenter
         }
     }


    HListView {
        id: sessionList
        anchors.fill: parent
        topMargin: theme.spacing / 2
        clip: true
        readonly property alias contextMenu: contextMenu
       // section.property: "display_name"
       // section.delegate: sectionDelegate

        model: LineEngine.getSessionModel()
        delegate: SessionDelegate {
            width:  sessionList.width
            height: theme.controls.avatar.size + theme.spacing

            //点击的时候需要手动修改currentindex， 选中效果
            onLeftClicked: {
                sessionList.currentIndex = model.index
                //打开右侧聊天面板
                clickSession(model)
                sessionList.model.current_index = sessionList.currentIndex;
            }

        }

        SessionContextMenu {
            onRemoveSession :{
                //通知信号上去，删除右侧面板
                root.removeSession(session)
                LineEngine.deleteSession(row, session.id)
                sessionList.currentIndex = -1
            }

            id: contextMenu
        }

        highlight: Item {
            readonly property alias border: border

            Rectangle {
                anchors.fill: parent
                color: theme.mainPane.accountBar.account.selectedBackground
                opacity: theme.mainPane.accountBar.account
                              .selectedBackgroundOpacity
            }

            Rectangle {
                id: border
                anchors.bottom: parent.bottom
                width: parent.width
                height:
                    theme.mainPane.accountBar.account.selectedBorderSize
                color: theme.mainPane.accountBar.account.selectedBorder
            }
        }
    }


    Behavior on implicitHeight { HNumberAnimation {} }
}
