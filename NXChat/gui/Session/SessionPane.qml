import QtQuick 2.15
import QtQuick.Controls 2.15

Page {
    //ContactBar 信号网上一直传递到顶部
    signal showChatPane(QtObject session)
    signal removeChatPane(QtObject session)
    //头部设置和搜索框
    header: TopBar {
    }

    //联系人列表
    SessionList {
        onClickSession: {
            showChatPane(session)
        }

        onRemoveSession: {
            removeChatPane(session)
        }
    }

    //底部先不要
    footer: BottomBar {
    }
}
