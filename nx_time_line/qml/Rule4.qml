import QtQuick 2.15
import models 1.0

Rectangle{
    id: rule

    height: 60
    width: parent.width

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

    Row{
        anchors{
            top: parent.top
            left: parent.left
        }

        Repeater{
            id: repeater

            model: instance.lineDatas
            Rectangle{
                width: modelData.position
                height: 60

                color: "transparent"

                RuleLineEdge{
                    anchors{
                        top: parent.top
                        right: parent.right
                    }

                    value: modelData.value
                    lineType: modelData.lineType
                }
            }
        }
    }

    function getCurPosFromMouseX(parentMouseX){
        return 0
    }



    function isMaximumScale(){
        return false
    }
}
