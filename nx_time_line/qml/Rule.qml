import QtQuick 2.15
import QtQuick.Controls.Basic

import models 1.0

Rectangle{
    id: rule
    property double initWidth: parent.width

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

    property double rPaddingVisible: 0
    property int rVisibleModel: 0
    property double rHighestUnitWidth: 0
    property int rHighestUnit : 0
    property int rVisibleOffset: 0

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

    property int totalInput : 500000

    Row{
        anchors{
            top: parent.top
        }

        x: rule.rPaddingVisible

        Repeater{
            id: repeater
            model: rule.rVisibleModel

            RuleLineStateItem {
                required property int index
                width: rule.rHighestUnitWidth
                ms: rule.rHighestUnit
                offset: index*rule.rHighestUnit+rule.rVisibleOffset
                onWidthChanged: {
                    if(index==0){
                        console.log("width "+width+"; ms: "+ms+"; offset:"+offset+"; padingVisible: "+rule.rPaddingVisible)
                    }
                }
            }

        }
    }


    onWidthChanged: {
        if(x===0){
            console.log("updateOnWidth width : "+width)
            update()
        }
    }

    onInitWidthChanged: {
        console.log("updateOnInitWidth initWidth : "+initWidth)
        update()
    }

    onXChanged: {
        console.log("updateOnXChanged x : "+x)
        update()
    }

    function update(){

        var widthPerMillis = width/totalInput
        const HIGHEST_VISIBLE_W = 200

        for(var i=0; i<msLevels.length-1;i++){
            console.log("widthPerMilis: "+widthPerMillis+"; i : "+i+"; msLevel: "+msLevels[i]+"; widthPerMili*msLevel[i]: "+(widthPerMillis*msLevels[i]))
            if(widthPerMillis*msLevels[i] < HIGHEST_VISIBLE_W &&  HIGHEST_VISIBLE_W < widthPerMillis*msLevels[i+1]){
                rHighestUnit = msLevels[i+1]
                console.log("i: "+i+" break")
                break
            }
            console.log("gan i+1: "+(i+1))
            rHighestUnit = msLevels[0]
        }

        console.log("rHighestUnit : "+rHighestUnit)

        rHighestUnitWidth = rHighestUnit*widthPerMillis

        console.log("padding : abs(x):"+Math.abs(x)+": ")
        var missingLeftUnit = Math.floor(Math.abs(x)/rHighestUnitWidth)
        console.log("paddingVisible : missingLeftUinit:"+missingLeftUnit+"; rHighestUnitWidth:"+rHighestUnitWidth)
        rPaddingVisible = missingLeftUnit*rHighestUnitWidth

        rVisibleModel = Math.floor(initWidth/rHighestUnitWidth)+2

        rVisibleOffset = missingLeftUnit*rHighestUnit

        console.log("widthPerMili :"+widthPerMillis+"; highestUnit:"+rHighestUnit+"; highestUnitWdith:"+rHighestUnitWidth+"; missingLeftUnit: "+missingLeftUnit+"; paddingVisible:"+rPaddingVisible)
        console.log("rVisibbleModel: "+rVisibleModel+"; rVisibleOfsset: "+rVisibleOffset)
    }

    function getCurPosFromMouseX(parentMouseX){
        var widthPerMillis = width/totalInput
        return Math.floor((parentMouseX+Math.abs(x))/widthPerMillis)
    }



    function isMaximumScale(){
        return rHighestUnit === msLevels[0] && rHighestUnitWidth > 800
    }
}
