import QtQuick 2.15
import QtQuick.Controls.Basic

import models 1.0

Rectangle{
    id: rule
    width: parent.width
    height: 60
    color: "transparent"
    anchors{
        top: parent.top
    }

    Rectangle {
        id: ruleHeader
        width: parent.width
        height: 40
        anchors{
            top: parent.top
            left: parent.left
        }
        color: "grey"
    }

    Repeater{
        id: prRpt
        model: [RuleLine.HIGHEST,RuleLine.NORMAL,RuleLine.SMALL,RuleLine.SMALLEST]
        Row {
            anchors{
                top: ruleHeader.bottom
                left: ruleHeader.left
            }
            Repeater {
                model: instance.ruleLines
                Rectangle{
                    width: instance.typeDistance(prRpt.model.modelData)
                    height: 30
                    color: "transparent"

                    Column{
                        anchors{
                            top: parent.top
                            left: parent.left
                        }
                        visible: index !== 0 && index !== (Math.floor(rule.width/10)-1)

                        Rectangle {
                            width: 1
                            height: getHeightFromType(modelData.type)
                            color: "white"
                            opacity: 1/(1+modelData.type)
                        }
                        Text{
                            width: parent.width
                            height: 15 // Adjust height according to your requirement
                            text: modelData.text // You can replace "Your Text Here" with index+"ms" if you have a variable named 'index'
                            color: "white"
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }
                }
            }
        }
    }

}
