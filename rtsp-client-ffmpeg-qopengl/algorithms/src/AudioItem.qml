import QtQuick 2.15

import Algorithms 1.0
import "Math.js" as CMath

Rectangle {
    id: audioItem
    width: 200
    height: 50

    property AudioConsumer consumer: null

    color: "lightgray"

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
        text: "PTS: " + CMath.formatDuration(consumer ? consumer.pts : 0)
    }

    Text {
        id: frameSampleRate
        anchors {
            left: parent.left
            bottom: parent.bottom
        }
        text: "Sample Rate: " + (consumer ? consumer.sampleRate : "No Sample Rate")
    }

    Text {
        id: frameChannels
        anchors {
            right: parent.right
            bottom: parent.bottom
        }
        text: "Channels: " + (consumer ? consumer.channels : "No Channels")
    }
}
