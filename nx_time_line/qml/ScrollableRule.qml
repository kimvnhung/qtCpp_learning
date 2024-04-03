import QtQuick 2.15

Item {
    width: 800
    height: 600
    visible: true

    // The scale factor for zooming
    property real scaleFactor: 1

    Rectangle {
        width: parent.width
        height: parent.height



        // Mouse area for detecting scroll wheel events
        MouseArea {
            anchors.fill: parent
            onWheel: {
                if (wheel.angleDelta.y > 0) {
                    // Zoom in
                    scaleFactor *= 1.1
                } else {
                    // Zoom out
                    scaleFactor *= 0.9
                }
            }
        }

        // Pinch area for detecting pinch gestures
        PinchArea {
            anchors.fill: parent
            pinch.target: parent
            pinch.minimumScale: 0.5
            pinch.maximumScale: 3

            onPinchUpdated: {
                scaleFactor *= pinch.scale
            }
        }

        // Content to be zoomed
        Rectangle {
            width: 200
            height: 200
            color: "lightblue"
            border.color: "blue"
            radius: 10
            transform: Scale {
                xScale: scaleFactor
                yScale: scaleFactor
            }
        }
    }
}
