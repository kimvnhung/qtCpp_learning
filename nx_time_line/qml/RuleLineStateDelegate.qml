import QtQuick 2.15
import QtQuick.Controls

import models 1.0

import Qt.labs.qmlmodels


Rectangle{
    id: root
    property int offset : 0
    property int unit
    // property int subCount
    // property int curCount
    property var model: [2,5,10]
    property int lineType
    property int index : 0

    width: parent.width
    height: parent.height
    color: "transparent"

    RuleLineEdge{
        anchors{
            top: parent.top
            left: parent.left
        }
        lineType: parent.lineType
        value: offset+root.index*parent.unit
        visible: getCurCount() === 1 || parent.index !== 0
        onValueChanged: {
            console.log("curCoun "+getCurCount()+"; subCount : "+getSubCount())
        }
    }

    Row{
        anchors {
            top: parent.top
            left: parent.left
        }

        Repeater {
            id: repeater
            property bool loaded: false

            model: [
                {}
            ]
            delegate: RuleStateDelegateChooser{
            }
        }
    }

    function getCurCount() {
        if(root.lineType == RuleLine.HIGHEST){
            return 1
        }
        return model[root.lineType-1]
    }

    function getSubCount() {
        console.log("model.length "+model.length)
        if(model.length <= root.lineType){
            return 0
        }

        return model[root.lineType]
    }
}
