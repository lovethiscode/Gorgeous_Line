import QtQuick 2.15
import QtQuick.Controls 2.15


Rectangle {
    color: "transparent"
    function showChatTab (session){
        var children = conversations.contentChildren;
        var showIndex = -1;
        for(var i=0;i<children.length;i++){
            if(children[i].session.id === session.id) {
                showIndex = i;
            }
        }
        if(showIndex == -1) {
            var chatComponet = Qt.createComponent("qrc:/gui/Chat/Conversation.qml")
            var chatObj = chatComponet.createObject(conversations, {session :session});
            conversations.addItem(chatObj);
            conversations.currentIndex = conversations.count - 1;
        } else {
            conversations.currentIndex = showIndex;
        }
    }

    function removeChatTab (session){
        var children = conversations.contentChildren;
        for(var i=0;i<children.length;i++){
            if(children[i].session.id === session.id) {
                conversations.removeItem(children[i])
                conversations.currentIndex = -1;
                return
            }
        }
    }

    SwipeView {
        id: conversations
        clip:true
        interactive: false
        anchors.fill: parent

        Component.onCompleted: {
            contentItem.highlightMoveDuration = 0
        }
    }
}
