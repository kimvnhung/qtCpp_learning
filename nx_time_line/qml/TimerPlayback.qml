import QtQuick 2.15
import QtQuick.Controls.Basic

import models 1.0

Item {

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
                // instance.curPos = rule.getCurPosFromMouseX(mouseX)
                instance.mouseX = mouseX
            }

            onWheel: {
                var ruleWidth = instance.ruleWidth
                var ruleX = instance.viewX
                // var cachedMouseX = mouseX
                if(wheel.angleDelta.y < 0 && ruleWidth <= parent.width){
                    return
                }

                if(wheel.angleDelta.y > 0 && rule.isMaximumScale())
                    return

                //calculate new ruleSize
                //fomule: xn = alphaN*xn_1+mouseX*(1-alphaN)
                //with: xn_1 is last x of rule
                //    : alphaN = newWidth/oldWidth
                var alpha0 = Math.pow(1.2,wheel.angleDelta.y/120)
                var wn_1 = ruleWidth
                var xn_1 = ruleX
                var wn = wn_1*alpha0
                if(wn <= parent.width){
                    wn = 1
                }
                var alphaN = wn/wn_1
                if(wn >= parent.width){
                    // rule.width = wn
                    // rule.x = alphaN*xn_1+mouseX*(1-alphaN)
                    instance.ruleWidth = wn
                    instance.viewX = alphaN*xn_1+mouseX*(1-alphaN)
                }else {
                    // rule.width = parent.width
                    // rule.x = 0
                    instance.ruleWidth = parent.width
                    instance.viewX = 0;
                }

                // instance.curPos = rule.getCurPosFromMouseX(cachedMouseX)

                //calculate scrollbar size
                if(wn/width > 100)
                    scrollbar.size = 0.01
                else
                    scrollbar.size = width/wn

                scrollbar.position = Math.abs(instance.viewX/instance.ruleWidth)
            }
        }

        onWidthChanged: {
            instance.viewWidth = width
        }

        RuleContextCapturedView {
            id: rule
            width: parent.width
            onWidthChanged: {
                instance.ruleWidth = width
                instance.viewX = 0
            }
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
                // rule.x = -position*rule.width
            }


        }


    }



}
