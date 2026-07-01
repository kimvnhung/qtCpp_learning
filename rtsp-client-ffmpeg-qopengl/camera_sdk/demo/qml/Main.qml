import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import CameraDemo 1.0

Window {
    visible: true
    width: 1280
    height: 720
    title: "Camera SDK Demo"

    Rectangle {
        anchors.fill: parent
        color: "#222"

        Column {
            id: contentColumn
            anchors.margins: 8
            spacing: 8

            MediaPlayer {
                id: player
            }

            Row {
                spacing: 8
                Button {
                    text: "Start"
                    onClicked: player.start(input.text)
                }
                Button {
                    text: "Stop"
                    onClicked: player.stop()
                }
                Button {
                    text: "Reconnect"
                    onClicked: {
                        player.stop();
                        player.start(input.text);
                    }
                }
                TextField {
                    id: input
                    text: "rtsp://172.25.222.203:8554/test2"
                    width: 480
                }
                Slider {
                    id: vol
                    from: 0.0
                    to: 1.0
                    value: 1.0
                    width: 200
                    onValueChanged: player.setVolume(value)
                }
                Text {
                    text: qsTr("Volume: ") + Math.round(vol.value * 100) + "%"
                }
            }

            Row {
                spacing: 16
                Column {
                    Text {
                        text: "FPS: " + player.fps
                    }
                    Text {
                        text: "Resolution: " + player.width + " x " + player.height
                    }
                    Text {
                        text: "URL: " + player.url
                    }
                }
            }

            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 8
                VideoItem {
                    id: video1
                    objectName: "video1"
                    width: contentColumn.width / 2 - 12
                    height: contentColumn.height - 160
                }
                VideoItem {
                    id: video2
                    objectName: "video2"
                    width: contentColumn.width / 2 - 12
                    height: contentColumn.height - 160
                }
                Component.onCompleted: {
                    // attach video items to the MediaPlayer
                    player.addVideoItem(video1);
                    player.addVideoItem(video2);
                }
            }
        }
    }
}
