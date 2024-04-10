import QtQuick 2.15
import models 1.0


Rectangle{
    id: root
    required property int unit
    required property int lineType
    property alias model: repeater.model
    property Component delegate: Item{}

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
