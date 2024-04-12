import QtQuick 2.15
import models 1.0


Rectangle{
    id: root
    required property int unit
    required property int lineType
    property alias model: repeater.model
    property Component delegate: Item{}

    // border{
    //     color: (unit===60000 )?"red":"transparent"
    //     width: (unit===60000)?1:0
    // }

    // onXChanged: {
    //     if(unit===60000){
    //         // console.log("width:"+width+"; x: "+x+"; lineType:"+lineType)
    //     }
    // }

    color: "transparent"

    RuleLineEdge{
        anchors{
            top: parent.top
            left: parent.right
        }
        lineType: root.lineType
        value: unit
    }

    Row{
        anchors {
            top: parent.top
            left: parent.left
        }
        Repeater {
            id: repeater
            delegate: root.delegate
        }
    }
}
