import QtQuick 2.15
import QtQuick.Controls.Basic

import models 1.0

Item {
    onWidthChanged: {
        // instance.ruleWidth = background.width
        // instance.viewWidth = background.width
    }

    Rectangle {
        id: background
        width: parent.width
        height: 400
        color: "black"
        clip: true

        MouseArea{
            anchors.fill: parent
            hoverEnabled: true
            onPositionChanged: {
                instance.curPos = rule.getCurPosFromMouseX(mouseX)
            }

            onWheel: {
                var cachedMouseX = mouseX
                if(wheel.angleDelta.y < 0 && rule.width <= parent.width){
                    return
                }

                if(wheel.angleDelta.y > 0 && rule.isMaximumScale())
                    return

                //calculate new ruleSize
                //fomule: xn = alphaN*xn_1+mouseX*(1-alphaN)
                //with: xn_1 is last x of rule
                //    : alphaN = newWidth/oldWidth
                var wn_1 = rule.width
                var xn_1 = rule.x
                var wn = wn_1*Math.pow(1.2,wheel.angleDelta.y/120)
                if(wn <= parent.width){
                    wn = 1
                }
                var alphaN = wn/wn_1
                if(wn >= parent.width){
                    rule.width = wn
                    rule.x = alphaN*xn_1+mouseX*(1-alphaN)
                }else {
                    rule.width = parent.width
                    rule.x = 0
                }

                instance.curPos = rule.getCurPosFromMouseX(cachedMouseX)

                //calculate scrollbar size
                // var scrollSize = 1/scale
                // scrollbar.size = scrollSize<=0.1 ? 0.1:scrollSize
                // scrollbar.position = (cachedMouseX-x)/rule.width-scrollbar.size/2
            }
        }

        Rule {
            id: rule
            width: parent.width
        }

        ScrollBar {
            id: scrollbar
            width: parent.width
            size: 1
            position: 0
            active: true
            orientation: Qt.Horizontal
            policy: ScrollBar.AlwaysOn
            anchors{
                bottom: parent.bottom
            }


            contentItem: Rectangle {
                implicitWidth: parent.width
                implicitHeight: 15
                color: scrollbar.pressed ? "#81e889" : "#c2f4c6"
                // Hide the ScrollBar when it's not needed.
                opacity: scrollbar.policy === ScrollBar.AlwaysOn || (scrollbar.active && scrollbar.size < 1.0) ? 0.75 : 0

                // Animate the changes in opacity (default duration is 250 ms).
                Behavior on opacity {
                    NumberAnimation {}
                }

                ScrollCenterThumbnail {

                }
            }

            onPositionChanged: {
                var maxPos = 1 - size
                // rule.x = position*(rule.width-parent.width)/maxPos
            }
        }


    }



}
