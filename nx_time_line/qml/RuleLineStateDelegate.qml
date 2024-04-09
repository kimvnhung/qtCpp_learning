import QtQuick 2.15
import QtQuick.Controls
import Qt.labs.qmlmodels


Rectangle{
    id: root
    property int offset : 0
    property int unit
    required property int subCount
    property int curCount
    property int lineType
    property int index : 0

    property Component component: null

    width: parent.width
    height: parent.height
    color: "transparent"

    Row{
        anchors {
            top: parent.top
            left: parent.left
        }

        Repeater {
            id: repeater
            model: subCount
            Loader{
                sourceComponent: root.component
                width: root.width/repeater.count
                height: root.height
                onLoaded: {
                    item.unit = root.unit/root.subCount
                    item.curCount = root.subCount
                    item.index = index
                    item.lineType = root.lineType+1
                }
            }
        }
    }

    RuleLineEdge{
        anchors{
            top: parent.top
            right: parent.right
        }
        lineType: parent.lineType
        value: offset+(root.index+1)*parent.unit
        visible: curCount==1 || parent.index !== parent.curCount-1
    }

    onIndexChanged: {
        console.log("rootIndex "+index+"; lineType:"+lineType)
    }
}
