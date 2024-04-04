import QtQuick 2.15
import QtQuick.Controls.Basic

Item {

    Rectangle {
        id: background
        width: 300
        height: 300
        color: "black"

        property real scaleFactor: 1

        ScrollBar {
            id: control
            width: parent.width
            size: background.scaleFactor
            position: 0.2
            active: true
            orientation: Qt.Horizontal
            policy: ScrollBar.AlwaysOn
            anchors{
                bottom: parent.bottom
            }

            contentItem: Rectangle {
                implicitWidth: parent.width
                implicitHeight: 23
                radius: width / 2
                color: control.pressed ? "#81e889" : "#c2f4c6"
                // Hide the ScrollBar when it's not needed.
                opacity: control.policy === ScrollBar.AlwaysOn || (control.active && control.size < 1.0) ? 0.75 : 0

                // Animate the changes in opacity (default duration is 250 ms).
                Behavior on opacity {
                    NumberAnimation {}
                }
            }
        }

        MouseArea{
            anchors.fill: parent
            hoverEnabled: true
            onPositionChanged: {
                console.log(mouse.x)
                console.log(mouse.y)
            }

            onWheel: {
                background.scaleFactor += wheel.angleDelta.y/1200
                if(background.scaleFactor >= 1){
                    background.scaleFactor = 1;
                }
                if(background.scaleFactor <= 0.1){
                    background.scaleFactor = 0.1
                }
                console.log(mouseX)
                console.log(mouseY)
            }
        }
    }

}
