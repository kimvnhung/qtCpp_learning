import QtQuick 2.15
import QtQuick.Shapes

Rectangle{
    id: root
    width: 120
    height: 25
    color: "transparent"

    property color fillColor: "#e8e8e8"

    Rectangle{
        id: fill
        width: parent.width
        height: parent.height-shape.height
        anchors{
            bottom: shape.top
            left: parent.left
        }

        color: root.fillColor
        radius: 2
    }

    Rectangle{
        width: parent.width/2
        height: parent.height/2
        color: root.fillColor

        anchors{
            bottom: fill.bottom
            left: parent.left
        }
    }

    Text {
        anchors{
            centerIn: fill
        }

        color: "black"
        font{
            bold: true
            pixelSize: 14
        }

        text: qsTr("0:00:19")
    }

    Canvas {
        id: shape
        width: 3
        height: 3
        anchors{
            left: parent.left
            bottom: parent.bottom
        }

        onPaint: {
            var context = getContext("2d");

            context.beginPath();
            context.moveTo(0,0);
            context.lineTo(0,3);
            context.lineTo(3,0);
            context.closePath();

            context.fillStyle = root.fillColor;
            context.fill();
        }
    }
}
