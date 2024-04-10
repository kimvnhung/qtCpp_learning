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

    property int totalInput : 30000000

    Row{
        Repeater{
            id: repeater
            model: roundWithHighestUnit()+1
            RuleLineStateItem {
                required property int index
                width: getHighestUnit()*widthPerMillisecond()
                ms: getHighestUnit()
                offset: index*getHighestUnit()

                onWidthChanged: {
                    // console.log("unit "+ms+"; offset "+offset)
                }

                onMsChanged: {
                    console.log("unit "+ms+"; offset "+offset)
                }
            }

        }
    }

    onWidthChanged: {
        // repeater.model = roundWithHighestUnit()+1
        console.log("highestU "+getHighestUnit())
    }

    function widthPerMillisecond(){
        return parent.width/totalInput
    }

    function roundWithHighestUnit(){
        //this is number of highest ruleline
        return Math.floor(totalInput/getHighestUnit())
    }

    function getHighestUnit(){
        const wPerMillisecond = widthPerMillisecond()
        console.log("wPermili "+wPerMillisecond)
        const HIGHEST_VISIBLE_W = 150

        let highestUnit

        if (wPerMillisecond*5000 > HIGHEST_VISIBLE_W) {//5s
            highestUnit = 5000;
        } else if (wPerMillisecond*10000 > HIGHEST_VISIBLE_W) {//10s
            highestUnit = 10000;
        } else if (wPerMillisecond*30000 > HIGHEST_VISIBLE_W) {//30s
            highestUnit = 30000;
        } else if (wPerMillisecond*60000 > HIGHEST_VISIBLE_W) {//1min
            highestUnit = 60000;
        } else if (wPerMillisecond*300000 > HIGHEST_VISIBLE_W) {//5min
            highestUnit = 300000;
        } else if (wPerMillisecond*600000 > HIGHEST_VISIBLE_W) {//10min
            highestUnit = 600000;
        } else if (wPerMillisecond*1800000 > HIGHEST_VISIBLE_W) {//30min
            highestUnit = 1800000;
        } else if (wPerMillisecond*3600000 > HIGHEST_VISIBLE_W) {//1h
            highestUnit = 3600000;
        } else if (wPerMillisecond*10800000 > HIGHEST_VISIBLE_W) {//3h
            highestUnit = 10800000;
        }

        return highestUnit
    }
}
