import QtQuick 2.15

import Algorithms 1.0

Item {
    id: root
    width: 420
    height: 72

    // Public properties
    property bool playing: false
    property double currentTime: 0   // in seconds
    property double duration: 0      // in seconds
    property int stepSeconds: 10   // seek step in seconds

    // Signals for external wiring
    signal playPauseToggled(bool playing)
    signal stopRequested
    signal seekRequested(int secondsDelta)
    signal seekTo(int seconds)

    function formatDuration(ms) {
        const hours = Math.floor(ms / 3600000);
        ms %= 3600000;

        const minutes = Math.floor(ms / 60000);
        ms %= 60000;

        const seconds = Math.floor(ms / 1000);
        const millis = ms % 1000;

        return String(hours).padStart(2, '0') + ":" + String(minutes).padStart(2, '0') + ":" + String(seconds).padStart(2, '0') + ":" + String(millis).padStart(3, '0');
    }

    Row {
        anchors.centerIn: parent
        spacing: 12

        // seek backward
        Rectangle {
            id: backBtn
            width: 44
            height: 44
            color: "#f4f4f4"
            radius: 6
            border.color: "#dcdcdc"
            Text {
                anchors.centerIn: parent
                text: "⏮"
                font.pixelSize: 22
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    var delta = -root.stepSeconds;
                    root.currentTime = Math.max(0, root.currentTime + delta);
                    root.seekRequested(delta);
                }
            }
        }

        // play / pause
        Rectangle {
            id: playBtn
            width: 56
            height: 56
            color: root.playing ? "#0078d4" : "#2d2d2d"
            radius: 8
            Text {
                anchors.centerIn: parent
                text: root.playing ? "⏸" : "▶"
                color: "white"
                font.pixelSize: 28
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    root.playing = !root.playing;
                    root.playPauseToggled(root.playing);
                }
            }
        }

        // Stop button
        Rectangle {
            id: stopBtn
            width: 44
            height: 44
            color: "#f4f4f4"
            radius: 6
            border.color: "#dcdcdc"
            Text {
                anchors.centerIn: parent
                text: "⏹"
                font.pixelSize: 22
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    root.currentTime = 0;
                    root.playing = false;
                    root.stopRequested();
                }
            }
        }

        // seek forward
        Rectangle {
            id: fwdBtn
            width: 44
            height: 44
            color: "#f4f4f4"
            radius: 6
            border.color: "#dcdcdc"
            Text {
                anchors.centerIn: parent
                text: "⏭"
                font.pixelSize: 22
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    var delta = root.stepSeconds;
                    root.currentTime = Math.min(root.duration > 0 ? root.duration : Infinity, root.currentTime + delta);
                    root.seekRequested(delta);
                }
            }
        }

        // time display and progress
        Column {
            anchors.verticalCenter: parent.verticalCenter
            spacing: 6

            Text {
                id: timeText
                text: formatDuration(root.currentTime) + " / " + (root.duration > 0 ? formatDuration(root.duration) : "--:--")
                font.pixelSize: 14
                color: "#333"
            }

            Rectangle {
                id: progressBar
                width: 220
                height: 8
                radius: 4
                color: "#e8e8e8"

                Rectangle {
                    id: progress
                    anchors.left: parent.left
                    height: parent.height
                    width: (root.duration > 0 ? (root.currentTime / root.duration) * parent.width : 0)
                    color: "#0078d4"
                    radius: 4
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (root.duration > 0) {
                            var pos = mouse.x / parent.width;
                            var t = Math.round(pos * root.duration);
                            root.currentTime = t;
                            root.seekTo(t);
                        }
                    }
                }
            }
        }
    }
}
