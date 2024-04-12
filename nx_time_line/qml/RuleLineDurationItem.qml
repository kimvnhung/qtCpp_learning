import QtQuick 2.15
import models 1.0

Rectangle{
    id: root
    property int value: 1 // ms
    property int lineType: RuleLine.UNDEFINED
    property int curHighestUnit: 5000
    property var activeRange: [0,5000]
    property Component delegate: Item{}

    readonly property var delegateStates : [
        [2,5,10], // 1s
        [2,3,2], // 1any
        [5,2,5], // 5s
        [5,2,3], // 5min
        [2,5,2], //10any
        [3,2,5], //30any
        [3,2,3] // 3h
    ]

    property var delegateState: delegateStates[0]

    color: "transparent"

    Row{
        anchors {
            top: parent.top
            left: parent.left
        }
        Repeater {
            id: repeater
            model: 2
            delegate: root.delegate
        }
    }

    function initRepeater(){
        var secs = curHighestUnit/1000
        var mins = curHighestUnit/(60*1000)
        var hours = curHighestUnit/(60*60*1000)
        if(secs == 1){
            delegateState = delegateStates[0]
        }else if(secs == 5){
            delegateState = delegateStates[2]
        }else if(mins == 5){
            delegateState = delegateStates[3]
        }else if(hours == 3){
            delegateState = delegateStates[6]
        }else if(secs == 10 || mins == 10 || hours == 10){
            delegateState = delegateStates[4]
        }else if(secs == 30 || mins == 30 || hours == 30){
            delegateState = delegateStates[5]
        }else if(secs == 1 || mins == 1 || hours == 1){
            delegateState = delegateStates[2]
        }

        if(curHighestUnit == value){
            lineType = RuleLine.HIGHEST
        }else if(curHighestUnit/delegateState[0] == value){
            lineType = RuleLine.NORMAL
        }else if(curHighestUnit/(delegateState[0]*delegateState[1]) == value){
            lineType = RuleLine.SMALL
        }else if(curHighestUnit/(delegateState[0]*delegateState[1]*delegateState[2]) == value){
            lineType = RuleLine.SMALLEST
        }else {
            lineType = RuleLine.UNDEFINED
        }

        if(lineType < delegateState.length)
            repeater.model = delegateState[lineType]

    }

    RuleLineEdge{
        anchors{
            top: parent.top
            right: parent.right
        }

        lineType: root.lineType
        value: value
    }
}
