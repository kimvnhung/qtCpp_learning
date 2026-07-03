import QtQuick 2.15

import Algorithms 1.0

Rectangle {
    id: videoItem
    width: 200
    height: 200

    property VideoConsumer consumer: null

    color: "green"

    Text {
        id: frameId
        anchors {
            top: parent.top
            left: parent.left
        }
        text: "Id: " + (consumer ? consumer.id : "No Consumer")
    }

    Text {
        id: framePts
        anchors {
            top: parent.top
            right: parent.right
        }
        text: "PTS: " + (consumer ? consumer.pts : "No PTS")
    }

    Text {
        id: frameWidth
        anchors {
            left: parent.left
            bottom: parent.bottom
        }
        text: "Width: " + (consumer ? consumer.width : "No Width")
    }

    Text {
        id: frameHeight
        anchors {
            right: parent.right
            bottom: parent.bottom
        }
        text: "Height: " + (consumer ? consumer.height : "No Height")
    }
}
