import QtQuick 2.15
import models 1.0

Rectangle{
    id: root
    required property TimeStep model
    property Component delegate: Item{}

    width: model.unit*model.context.widthPerMili
    height: 15
    color: "transparent"

    onWidthChanged: {
        console.log("width "+width)
    }

    Row{
        anchors {
            top: parent.top
        }

        Repeater{
            model: root.model.subItems
            delegate: root.delegate
        }
    }

    RuleLineEdge{
        anchors{
            top: parent.top
            right: parent.right
        }

        lineType: model.lineType
        value: model.value
        visible: !model.isDisableText

        onValueChanged: {
            console.log("value "+value+"; lineType : "+lineType)
        }
    }
}
