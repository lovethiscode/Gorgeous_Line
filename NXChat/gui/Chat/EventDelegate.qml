import QtQuick 2.15
import QtQuick.Layouts 1.15

import "../Base"
import "../Base/HTile"
import "EventItem"
import EventModel 0.1
import SessionModel 0.1
import EnumDefine 0.1

HRowLayout {
    id: eventContent
    property QtObject session;
    readonly property bool onRight: model.isMe
    property QtObject eventItem: null
    layoutDirection: onRight ? Qt.RightToLeft: Qt.LeftToRight

    Item {
        id: avatarWrapper
        Layout.alignment: Qt.AlignCenter
        Layout.preferredHeight: Layout.preferredWidth
        Layout.preferredWidth:  theme.chat.message.avatarSize

        HUserAvatar {
            anchors.fill: parent
            id: avatar
            clientUserId: model.id
            userId: model.sender_id
            displayName: model.sender_name
            mxc: model.avatar_url
            width: parent.width
            height: parent.Layout.preferredWidth
            radius: theme.chat.message.avatarRadius
        }
    }

    function createEventText() {
        var chatComponet = Qt.createComponent("qrc:/gui/Chat/EventItem/EventImageTextBubble.qml")
        return chatComponet.createObject(title, { "font.pixelSize": theme.fontSize.middle,
                                            backgroundColor:onRight ? utils.makeRgb(152, 225, 101) : Qt.hsla(0, 0, 0, 0.7),
                                            color: onRight ? "black" : "white",
                                            "Layout.fillWidth": true,
                                            horizontalAlignment: onRight? TextEdit.AlignRight : TextEdit.AlignLeft,
                                            textFormat: Text.StyledText,
                                            wrapMode: Text.Wrap,
                                            model: model,
                                            session_id :session.id,
                                            "Layout.alignment": onRight ? Qt.AlignRight : Qt.AlignLef
                                        });
    }

    function createEventImage() {
        var chatComponet = Qt.createComponent("qrc:/gui/Chat/EventItem/EventImage.qml")
        return chatComponet.createObject(title, {"Layout.alignment": onRight ? Qt.AlignRight : Qt.AlignLef,
                                      width: implicitWidth,
                                      session_id :session.id,
                                      model: model});
    }

    function createEventAudio() {
        var chatComponet = Qt.createComponent("qrc:/gui/Chat/EventItem/EventAudio.qml")
        return chatComponet.createObject(title, {"Layout.alignment": onRight ? Qt.AlignRight : Qt.AlignLef,
                                             model: model,
                                             session_id :session.id});
    }

    function createEventVideo() {
        var chatComponet = Qt.createComponent("qrc:/gui/Chat/EventItem/EventVideo.qml")
        return chatComponet.createObject(title, {
                                             "Layout.alignment": onRight ? Qt.AlignRight : Qt.AlignLef,
                                             model: model,
                                             session_id :session.id});
    }

    function createEventFile() {
        var chatComponet = Qt.createComponent("qrc:/gui/Chat/EventItem/EventFile.qml")
        return chatComponet.createObject(title, {"Layout.alignment": onRight ? Qt.AlignRight : Qt.AlignLef,
                                      model: model,
                                      session_id :session.id});
    }


    function createEventContact() {
        var chatComponet = Qt.createComponent("qrc:/gui/Chat/EventItem/EventContact.qml")
        return chatComponet.createObject(title, {"Layout.alignment": onRight ? Qt.AlignRight : Qt.AlignLef,
                                      model: model,
                                      session_id :session.id});
    }



    function openContextMenu() {
        eventList.contextMenu.spawn(model)
    }


    //名字和内容
    HColumnLayout {
        id: title
        Layout.maximumWidth: parent.width /3 * 2

       Component.onCompleted: {          
           if(model.type === EnumDefine.EventTypeText) {
                eventItem = createEventText()
            } else if(model.type === EnumDefine.EventTypeImage) {
              eventItem = createEventImage()
            } else if(model.type === EnumDefine.EventTypeAudio) {
                eventItem = createEventAudio()
            } else if(model.type === EnumDefine.EventTypeVideo) {
                eventItem = createEventVideo()
            } else if (model.type === EnumDefine.EventTypeFile) {
                eventItem = createEventFile()
            } else if (model.type === EnumDefine.EventTypeContact || model.type === EnumDefine.EventTypeVcard) {
               eventItem = createEventContact()
            }
        }

        TitleLabel {
            id: display
            text: model.sender_name || model.sender_id
            font.pixelSize: theme.fontSize.big
            color:  utils.nameColor(model.display_name || model.id.substring(1))
            horizontalAlignment: onRight ? Qt.AlignRight : Qt.AlignLeft
            visible: session.type === EnumDefine.SessionTypeGroup
        }

        //创建一个右键点击区域
        Rectangle {
            x: eventItem.x
            y: eventItem.y
            width: eventItem.width
            height: eventItem.height
            color: "transparent"
            z: 1000

            TapHandler {
                acceptedButtons: Qt.RightButton
                acceptedPointerTypes: PointerDevice.GenericPointer | PointerDevice.Pen
                onTapped: openContextMenu()
            }
        }
    }
}

