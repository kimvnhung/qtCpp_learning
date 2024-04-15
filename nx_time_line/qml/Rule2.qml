import QtQuick 2.15

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

    property int totalInput: 100000
    property int initRepeaterModel: 0
    property int initHighestUnitWidth: 0
    property int initHighestUnit: 0
    property int highestUnit: 0
    property double widthPerMili: 0

    Row {
        anchors{
            top: parent.top
            left: parent.left
        }

        Repeater {
            id: repeater
            model: initRepeaterModel
            RuleLineDurationItem{
                width: initHighestUnitWidth
                value: initHighestUnit
                curHighestUnit: highestUnit
                delegate: RuleLineDurationDelegateChooser{
                    curHighestUnit: highestUnit
                }
                onWidthChanged: {
                    console.log("indx:"+index)
                }
            }
            onModelChanged: {
                console.log("model "+model)
            }
        }
    }

    readonly property var msLevels: [
        5000,
        10000,
        30000,
        60000,
        300000,
        600000,
        1800000,
        3600000,
        10800000
    ]

    function update(){
        widthPerMili = width/totalInput
        const HIGHEST_VISIBLE_W = 200

        var i = 0;
        for(i=0; i<msLevels.length-1;i++){
            if(widthPerMili*msLevels[i] < HIGHEST_VISIBLE_W &&  HIGHEST_VISIBLE_W < widthPerMili*msLevels[i+1]){
                highestUnit = msLevels[i+1]
                break
            }
            if(i==msLevels.length-2)
                highestUnit = msLevels[0]
        }
    }

    onHighestUnitChanged: {
        if(initHighestUnit == 0)
            initHighestUnit = highestUnit

        console.log("highestUnit : "+highestUnit)
    }

    onInitHighestUnitChanged: {
        initHighestUnitWidth = widthPerMili*initHighestUnit
    }

    onWidthPerMiliChanged: {
        initHighestUnitWidth = widthPerMili*initHighestUnit
    }

    onInitHighestUnitWidthChanged: {
        if(initHighestUnitWidth != 0){
            if(initRepeaterModel == 0){
                console.log("totalInput: "+totalInput+"; initHighestUnit :"+initHighestUnit)
                initRepeaterModel = Math.floor(totalInput/initHighestUnit)
            }
        }
    }

    onWidthChanged: {
        update()
    }

    function getCurPosFromMouseX(parentMouseX){
        return Math.floor((parentMouseX+Math.abs(x))/widthPerMili)
    }

    function isMaximumScale(){
        return highestUnit === msLevels[0] && highestUnit*widthPerMili > 800
    }
}
