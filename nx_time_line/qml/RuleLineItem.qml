import QtQuick 2.15

import models 1.0


Rectangle {
    property int lineType: RuleLine.UNDEFINED
    property int msValue: 100
    width: parent.width
    height: 100
    color: "green"
    Row {
        anchors{
            top: parent.top
            left: parent.left
        }
        RuleLineSubItem{
            height: getHeightFromType(lineType)
        }
        RuleLineSubItem{
            height: getHeightFromType(lineType)
        }
    }

    function getHeightFromType(type) : real {
        switch(type){
        case RuleLine.HIGHEST:
            return 15
        case RuleLine.NORMAL:
            return 11
        case RuleLine.SMALL:
            return 7
        case RuleLine.SMALLEST:
            return 3
        default:
            return 0
        }
    }

    onLineTypeChanged: {
        console.log(lineType)
        console.log(RuleLine.HIGHEST)
        console.log(RuleLine.UNDEFINED)
    }
}
