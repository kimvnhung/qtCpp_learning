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
            // Loader{
            //     sourceComponent: root.component
            //     // source: loaded ? null:"RuleLineStateDelegate.qml"
            //     width: root.width/repeater.count
            //     height: root.height
            //     onLoaded: {
            //         if(root.lineType != RuleLine.SMALLEST){
            //             loaded = true
            //         }

            //         item.unit = root.unit/root.getSubCount()
            //         item.index = index
            //         item.lineType = root.lineType+1
            //         item.offset = root.offset+index*item.unit
            //     }
            // }
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
