import QtQuick 2.15
import QtQuick.Controls.Basic

import models 1.0

Rectangle{
    id: rule
    property double initWidth: parent.width

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
        anchors{
            top: parent.top
        }

        x: getPaddingVisible()

        Repeater{
            id: repeater
            model: visibleModel()

            RuleLineStateItem {
                required property int index
                width: getHighestUnitWidth()
                ms: getHighestUnit()
                offset: index*getHighestUnit()+getVisibleOffset()
            }

        }
    }


    onWidthChanged: {
        console.log("padding : "+getPaddingVisible()+"; offset "+getVisibleOffset()+"; higehstUnit:"+getHighestUnit()+"; ruleWidth:"+rule.width+"; ruleX: "+rule.x)
    }

    function getCurPosFromMouseX(parentMouseX){
        return Math.floor((parentMouseX+Math.abs(x))/widthPerMillisecond())
    }

    function getVisibleOffset(){
        return getMissingLeftUnit()*getHighestUnit()
    }

    function getPaddingVisible(){
        return getMissingLeftUnit()*getHighestUnitWidth()
    }

    function visibleModel() {
        return Math.floor(initWidth/getHighestUnitWidth())+2
    }

    function paddingVisible() {
        return Math.floor(Math.abs(x)/(widthPerMillisecond()*getHighestUnit()))*widthPerMillisecond()*getHighestUnit()
    }

    function getOffsetWithPadding(){
        return Math.round(paddingVisible()/(widthPerMillisecond()*getHighestUnit()))*getHighestUnit()
    }



    function getMissingLeftUnit(){
        return Math.floor(Math.abs(x)/getHighestUnitWidth())
    }

    function widthPerMillisecond(){
        return width/totalInput
    }

    function roundWithHighestUnit(){
        //this is number of highest ruleline
        return Math.floor(totalInput/getHighestUnit())
    }

    function getHighestUnitWidth(){
        return widthPerMillisecond()*getHighestUnit()
    }

    function isMaximumScale(){
        return getHighestUnit() === 5000 && getHighestUnitWidth() > 800
    }

    function getHighestUnit(){
        const wPerMillisecond = widthPerMillisecond()
        const HIGHEST_VISIBLE_W = 200

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
