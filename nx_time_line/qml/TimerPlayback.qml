import QtQuick 2.15
import QtQuick.Controls.Basic

import models 1.0

Item {
    Component.onCompleted: {
        instance.ruleWidth = background.width
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
            }

            onWheel: {
                if(wheel.angleDelta.y < 0 && rule.width <= parent.width){
                    return
                }

                //calculate new ruleSize
                var ruleSize = rule.width
                ruleSize *= 1 + wheel.angleDelta.y/1200
                if(ruleSize <= parent.width){
                    ruleSize = 1
                }
                var scale = ruleSize/parent.width
                if(ruleSize >= parent.width){
                    instance.ruleWidth = ruleSize
                    rule.x = mouseX*(1-scale)
                }else {
                    instance.ruleWidth = parent.width
                    rule.x = 0
                }


                //calculate scrollbar size
                var scrollSize = 1/scale
                scrollbar.size = scrollSize<=0.1 ? 0.1:scrollSize

                scrollbar.position = (mouseX-x)/rule.width-scrollbar.size/2
            }
        }

        Rule {
            id: rule
            width: instance.ruleWidth
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
                rule.x = position*(rule.width-parent.width)/maxPos
            }
        }


    }

    function getHeightFromType(type) : real {
        switch(type){
        case 0: //RuleLineType.HIGHEST:
            return 15
        case 1: //RuleLineType.NORMAL:
            return 11
        case 2: //RuleLineType.SMALL:
            return 7
        case 3: //RuleLineType.SMALLEST:
            return 3
        default:
            return 0
        }
    }

}
