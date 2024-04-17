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

    // Behavior on width {
    //     enabled: targetValue >= parent.width && parent.width > 0
    //     PropertyAnimation{
    //         duration: 200
    //         easing.type: Easing.Linear
    //     }
    // }

    // Behavior on x {
    //     enabled: x < 0
    //     PropertyAnimation{
    //         duration: 200
    //         easing.type: Easing.Linear
    //     }
    // }

    Row{
        anchors {
            top: ruleHeader.bottom
        }

        Repeater{
            id: repeater

            model: instance.timeSteps
            TimeStepItem{
                required property var modelData
                model: modelData
                delegate: TimeStepItemChooser{

                }
            }

            onModelChanged: {
                console.log("model count "+model.length)
                if(model.length > 0){
                    console.log("value : "+model[0].value)
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
