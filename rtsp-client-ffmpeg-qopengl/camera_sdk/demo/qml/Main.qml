import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import Rtsp 1.0

Window {
    visible: true
    width: 1280
    height: 720
    title: "Camera SDK Demo"

    property alias bridge: sdkBridge

    Rectangle {
        anchors.fill: parent
        color: "#222"

        Row {
            anchors.fill: parent
            spacing: 8

            Column {
                anchors.fill: parent
                anchors.margins: 8
                spacing: 8

                Row {
                    spacing: 8
                    Button { text: "Start"; onClicked: sdkBridge.start(input.text) }
                    Button { text: "Stop"; onClicked: sdkBridge.stop() }
                    Button { text: "Reconnect"; onClicked: sdkBridge.reconnect() }
                    TextField { id: input; text: "rtsp://172.25.222.203:8554/test2"; width: 480 }
                }

                Row {
                    spacing: 16
                    Column {
                        Text { text: "FPS: " + sdkBridge.fps }
                        Text { text: "Resolution: " + sdkBridge.width + " x " + sdkBridge.height }
                        Text { text: "Codec: " + sdkBridge.codec }
                    }
                }

                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 8
                    VideoItem { id: video1; objectName: "video1"; width: parent.width/2 - 12; height: parent.height - 160 }
                    VideoItem { id: video2; objectName: "video2"; width: parent.width/2 - 12; height: parent.height - 160 }
                }
            }
        }
    }
}
