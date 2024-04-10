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

    property int totalInput : 5000

    Row{
        Repeater{
            id: repeater
            model: 5
            RuleLineStateItem {
                required property int index
                width: rule.width/repeater.count
                ms: rule.totalInput/repeater.count
                offset: index*rule.totalInput/repeater.count

                onWidthChanged: {
                    console.log("idnex "+index)
                }
            }

        }
    }

    function modelFromTotalInput(){
        const widthPerMillisecond = parent.width/totalInput
        const HIGHEST_VISIBLE_W = 50

        let highestUnit

        if (widthPerMillisecond*5000 > HIGHEST_VISIBLE_W) {//5s
            highestUnit = 5000;
        } else if (widthPerMillisecond*10000 > HIGHEST_VISIBLE_W) {//10s
            highestUnit = 10000;
        } else if (widthPerMillisecond*30000 > HIGHEST_VISIBLE_W) {//30s
            highestUnit = 30000;
        } else if (widthPerMillisecond*60000 > HIGHEST_VISIBLE_W) {//1min
            highestUnit = 60000;
        } else if (widthPerMillisecond*300000 > HIGHEST_VISIBLE_W) {//5min
            highestUnit = 300000;
        } else if (widthPerMillisecond*600000 > HIGHEST_VISIBLE_W) {//10min
            highestUnit = 600000;
        } else if (widthPerMillisecond*1800000 > HIGHEST_VISIBLE_W) {//30min
            highestUnit = 1800000;
        } else if (widthPerMillisecond*3600000 > HIGHEST_VISIBLE_W) {//1h
            highestUnit = 3600000;
        } else if (widthPerMillisecond*10800000 > HIGHEST_VISIBLE_W) {//3h
            highestUnit = 10800000;
        }

        return Math.floor(totalInput/highestUnit)
    }
}
