import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15

import Algorithms 1.0

ApplicationWindow {
    id: root
    visible: true
    width: 800
    height: 600
    // make center in screen
    x: 0
    y: 0
    title: qsTr("Simple Window")

    Component.onCompleted: {
        console.log("ApplicationWindow completed");
        mediaPlayer.open("file_stream");
        mediaPlayer.addVideoOutput(videoConsumer1);
        mediaPlayer.addVideoOutput(videoConsumer2);
    }

    VideoConsumer {
        id: videoConsumer1
    }

    VideoConsumer {
        id: videoConsumer2
    }

    MediaPlayer {
        id: mediaPlayer
    }

    Rectangle {
        anchors.fill: parent

        Column {
            anchors.centerIn: parent
            spacing: 20

            Row {
                spacing: 20

                ControlPlayback {
                    id: playbackControl
                    width: 400
                    height: 72
                    playing: mediaPlayer.playing
                    currentTime: mediaPlayer.currentTime
                    duration: mediaPlayer.duration
                    onPlayPauseToggled: {
                        if (mediaPlayer.playing) {
                            mediaPlayer.pause();
                        } else {
                            mediaPlayer.play();
                        }
                    }
                    onStopRequested: {
                        mediaPlayer.stop();
                    }
                }

                AudioItem {
                    id: audioItem
                    width: 200
                    height: 50
                    consumer: mediaPlayer.audioOutput
                }
            }

            Row {
                spacing: 20
                VideoItem {
                    id: videoItem1
                    width: 200
                    height: 200
                    consumer: videoConsumer1
                }

                VideoItem {
                    id: videoItem2
                    width: 200
                    height: 200
                    consumer: videoConsumer2
                }
            }
        }
    }
}
