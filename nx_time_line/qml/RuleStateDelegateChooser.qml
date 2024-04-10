import QtQuick 2.15
import Qt.labs.qmlmodels
import models 1.0


DelegateChooser{
    id: chooser
    role: 'type'

    property double width: 0
    property double height: 0

    DelegateChoice{
        roleValue: 'leaf'

        Rectangle{
            required property var modelData
            width: modelData.width
            height: chooser.height
            color: "transparent"

            RuleLineEdge{
                anchors{
                    top: parent.top
                    left: parent.right
                }
                lineType: modelData.lineType
                value: modelData.value
            }
        }
    }

    DelegateChoice{
        roleValue: 'node'

        RuleLineStateDelegate2{
            required property var modelData
            width: modelData.width
            height: chooser.height
            unit: modelData.value
            lineType: modelData.lineType
            delegate: chooser
            model: modelData.children

        }
    }
}
