import QtQuick 2.15
import models 1.0
import QtQuick.Controls 2.15

Item {
    width: 300
    height: 300

    property real speed: instance.speed // Speed of rotation in degrees per frame
    property int numHands: instance.numHands

    Timer {
        interval: 16 // Adjust interval to control frame rate (16ms for ~60fps)
        running: true
        repeat: true
        onTriggered: fanHand.rotation += speed
    }

    Row{
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        spacing: 20

        Text {
            text: "Speed: " + speed.toFixed(2) + " degrees/frame"
            font.pixelSize: 16
        }


        Text {
            text: "Number of Hands: " + numHands
            font.pixelSize: 16
        }
    }

    Rectangle {
        id: fanHand
        width: 150
        height: 150
        radius: 75
        color: "transparent"
        border.color: "black"
        anchors.centerIn: parent

        Repeater {
            model: numHands
            Rectangle {
                width: 5
                height: fanHand.height
                color: "black"
                anchors.centerIn: parent

                rotation: model.index * (180/numHands)
            }
        }
    }
}
