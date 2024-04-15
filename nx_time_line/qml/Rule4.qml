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

    // Behavior on x {
    //     PropertyAnimation{
    //         duration: 500
    //     }
    // }

    Behavior on width {
        enabled: targetValue >= parent.width && parent.width > 0
        PropertyAnimation{
            duration: 500
        }
    }

    Repeater{
        id: repeater

        model: instance.lineDatas
        RuleLineEdge{
            anchors{
                top: parent.top
            }

            x: modelData.position

            value: modelData.value
            lineType: modelData.lineType


        }
    }

    function getCurPosFromMouseX(parentMouseX){
        return 0
    }



    function isMaximumScale(){
        return false
    }
}
