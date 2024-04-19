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
                // instance.curPos = rule.getCurPosFromMouseX(mouseX)
                instance.mouseX = mouseX
                if(pressed){
                    if(mouseX > mediaCursor.x)
                        mediaCursor.width = mouseX-mediaCursor.x
                    else
                    {
                        var oldWidth = mediaCursor.width
                        mediaCursor.width = mediaCursor.x-mouseX+oldWidth
                        mediaCursor.x = mouseX
                    }
                }
            }

            onPressed: {
                mediaCursor.width = 2
                mediaCursor.x = mouseX
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
            id: mediaCursor
            width: 100
        }
    }



}
