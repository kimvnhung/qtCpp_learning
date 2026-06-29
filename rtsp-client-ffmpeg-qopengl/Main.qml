import QtQuick 2.15
import QtQuick.Window 2.15
import RtspClientFFmpegQOpenGL 0.1

Window {
    visible: true
    width: 1280
    height: 720
    title: "RTSP Video"

    VideoItem {
        id: video
        objectName: "video"
        anchors.fill: parent
    }
}
