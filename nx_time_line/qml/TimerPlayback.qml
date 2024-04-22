import QtQuick 2.15
import QtQuick.Controls.Basic

import models 1.0
import "math_utils.js" as MMath

Item {
    id: root
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
                hoverCursor.x = mouseX

                instance.mouseX = mouseX
                if(pressed){
                    if(mouseX > runningCusor.x)
                        runningCusor.width = mouseX-runningCusor.x
                    else
                    {
                        var oldWidth = runningCusor.width
                        runningCusor.width = runningCusor.x-mouseX+oldWidth
                        runningCusor.x = mouseX
                    }
                }


            }

            onPressed: {
                runningCusor.width = 2
                runningCusor.x = mouseX
            }

            onEntered: {
                hoverCursor.visible = true
            }

            onExited: {
                hoverCursor.visible = false
            }

            onWheel: {
                var ruleWidth = instance.ruleWidth
                var ruleX = instance.viewX

                // var cachedMouseX = mouseX

                if(wheel.angleDelta.y < 0 && ruleWidth <= parent.width){
                    return
                }

                if(wheel.angleDelta.y > 0 && instance.isMaximumScale)
                    return

                //calculate new ruleSize
                //fomule: xn = alphaN*xn_1+mouseX*(1-alphaN)
                //with: xn_1 is last x of rule
                //    : alphaN = newWidth/oldWidth
                var alpha0 = Math.pow(1.2,wheel.angleDelta.y/120)
                var wn_1 = ruleWidth
                var xn_1 = ruleX
                var wn = wn_1*alpha0
                if(MMath.differentPercent(wn,parent.width) < 15){
                    wn = 1
                    instance.ruleWidth = parent.width
                    instance.viewX = 0
                }else {//over 20%
                    var alphaN = wn/wn_1
                    instance.ruleWidth = wn
                    instance.viewX = alphaN*xn_1+mouseX*(1-alphaN)
                }

                // instance.curPos = rule.getCurPosFromMouseX(cachedMouseX)

                //calculate scrollbar size
                if(wn/width > 100)
                    scrollbar.size = 0.01
                else
                    scrollbar.size = parent.width/wn

                scrollbar.position = Math.abs(instance.viewX/instance.ruleWidth)
            }
        }

        onWidthChanged: {
            instance.viewWidth = width
        }

        RuleContextCapturedView {
            id: rule
            width: parent.width
            height: 55

            anchors{
                bottom: timeZone.top
                left: parent.left
            }

            onWidthChanged: {
                instance.ruleWidth = width
                instance.viewX = 0
            }
        }

        TimeZoneRuler{
            id: timeZone
            anchors{
                bottom: scrollbar.top
                left: parent.left
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
                color: scrollbar.pressed ? "#606060" : (scrollbar.hovered ? "#686868" : "#585858")
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
                instance.viewX = -position*instance.ruleWidth
                // if(position == 0)
                //     background.x = 0
                // else if(position == 1-size && size !== 1)
                //     background.x = -200
            }

            onSizeChanged: {
                // if(size === 1)
                // {
                //     background.width = root.width
                //     background.x = 0
                // }else{
                //     background.width = root.width+200
                //     background.x = -100
                // }
            }
        }

        MediaCursor{
            id: runningCusor

            anchors{
                top: rule.top
                bottom: parent.bottom
            }
        }

        MediaCursor {
            id: hoverCursor

            anchors{
                top: rule.top
                bottom:  parent.bottom
            }
        }

        TimePositionBubble{
            id: positionBubble
            anchors{
                bottom: rule.top
                left: runningCusor.right
            }
        }
    }



}
