import QtQuick 2.15
import Qt.labs.qmlmodels
import models 1.0


DelegateChooser{
    id: chooser
    role: 'type'

    required property double height

    DelegateChoice{
        roleValue: 'leaf'

        Rectangle{
            required property var modelData
            width: modelData.width
            height: chooser.height
            color: "transparent"

            border{
                color: (modelData.value==60000)?"green":"transparent"
                width: (modelData.value==60000)?1:0
            }

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
