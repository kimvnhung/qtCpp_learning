import QtQuick 2.15
import QtQuick.Controls.Basic

Item {

    Rectangle {
        id: background
        width: 300
        height: 300
        color: "black"
        clip: true

        property real scaleFactor: 1
        property real scrollPos: 0
        property real maxPos: 1-scaleFactor

        MouseArea{
            anchors.fill: parent
            hoverEnabled: true
            onPositionChanged: {
            }

            onWheel: {
                background.scaleFactor += wheel.angleDelta.y/1200
                if(background.scaleFactor >= 1){
                    background.scaleFactor = 1;
                }
                if(background.scaleFactor <= 0.1){
                    background.scaleFactor = 0.1
                }

                background.scrollPos = mouseX/width

            }
        }

        Rectangle{
            id: rule
            width: parent.width/background.scaleFactor
            height: 60
            anchors{
                top: parent.top
            }
            color: "grey"

            onWidthChanged: {
                console.log("rule.Widt "+parseInt(width/20))
            }

            Row {
                id: lv1
                Repeater {
                    model: Math.floor(rule.width/10)
                    Rectangle{
                        width: 10
                        Rectangle {
                            width: 2
                            height: 20
                            color: "green"
                            anchors{
                                top: parent.top
                                left: parent.left
                            }
                        }
                    }
                }
            }

        }

        ScrollBar {
            id: control
            width: parent.width
            size: background.scaleFactor
            position: background.scrollPos
            active: true
            orientation: Qt.Horizontal
            policy: ScrollBar.AlwaysOn
            anchors{
                bottom: parent.bottom
            }


            contentItem: Rectangle {
                implicitWidth: parent.width
                implicitHeight: 15
                color: control.pressed ? "#81e889" : "#c2f4c6"
                // Hide the ScrollBar when it's not needed.
                opacity: control.policy === ScrollBar.AlwaysOn || (control.active && control.size < 1.0) ? 0.75 : 0

                // Animate the changes in opacity (default duration is 250 ms).
                Behavior on opacity {
                    NumberAnimation {}
                }

                ScrollCenterThumbnail {

                }
            }

            onPositionChanged: {
                rule.x = position*(rule.width-parent.width)/background.maxPos
            }
        }


    }

}
