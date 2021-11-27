import QtQuick 2.15
import QtQuick.Controls 2.15
import "Base"
import "Base/Buttons"

ApplicationWindow {
    id: window
    //flags: Qt.WA_TranslucentBackground
    minimumWidth: theme ? theme.minimumSupportedWidth : 240
    minimumHeight: theme ? theme.minimumSupportedHeight : 120
    width: Math.min(screen.width, 1152)
    height: Math.min(screen.height, 768)
    visible: true
    color: "transparent"
    property var theme: null
    property var mainUI: null

    property var settings: ({
                            Keys : {
                                zoom_in: ["Ctrl++"],
                                zoom_out: ["Ctrl+-"],
                                reset_zoom : ["Ctrl+="],
                                quit : ["Alt+F4"],
                                compact : ["Alt+Ctrl+C"],
                                notifications_mute : ["Ctrl+Alt+N"],
                                notifications_highlights_only : ["Ctrl+Alt+H"],
                                Accounts : {
                                    add : ["Ctrl+A"]
                                }
                            },
                            General: {
                                zoom : 1.0,
                                tooltips_delay: 0.7
                            },
                            Accounts: [],
                            Rooms: [],
                            RoomList: {
                                min_width : 144
                            },
                            Scrolling : {
                                kinetic : false
                            },
                            Notifications : {
                                start_level : "enable"
                            },
                            Chat : {
                                Files : {
                                    auto_play_gif : true
                                }
                            },
                            Presence : {
                                saved_status : 5
                            }
                        })

    property bool ready: false
    Component.onCompleted: {
        //加载 皮肤文件
        var component = Qt.createComponent("qrc:/gui/Glass.qml");

        window.theme = component.createObject(window, {uiScale : 0.8})
        //设置背景图片
        window.theme.ui.image = "qrc:/resource/midnight.jpg"

        utils.theme = window.theme
        ready = true
    }

    Utils { id: utils }
    //loading 页面
    HLoader {
        anchors.fill: parent
        source: ready ? "" : "LoadingScreen.qml"
    }
    //主面板
    HLoader {
       // true makes the initially loaded chat page invisible for some reason
       asynchronous: false

       anchors.fill: parent
       focus: true
       scale: ready ? 1 : 0.5
       source: ready ? "MainUI.qml" : ""


       Behavior on scale { HNumberAnimation { overshoot: 3; factor: 1.2 } }
   }

    function makePopup(urlComponent, properties={}, callback=null, autoDestruct=true,) {
        utils.makePopup(urlComponent, window, properties, callback, autoDestruct,
        )
    }
}
