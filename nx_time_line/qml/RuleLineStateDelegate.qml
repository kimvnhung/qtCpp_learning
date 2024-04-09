import QtQuick 2.15
import QtQuick.Controls
import Qt.labs.qmlmodels


// Rectangle{
//     id: root


//     DelegateChooser{
//         id: chooser
//         role: 'type'

//         DelegateChoice{
//             roleValue: 'data'
//             Rectangle{
//                 width: root.width
//                 RuleLineEdge{
//                     anchors{
//                         top: parent.top
//                         right: parent.right
//                     }
//                     lineType: root.lineType
//                 }
//             }
//         }
//     }

// }
Rectangle{
    id: root
    property int offset : 0
    required property int ms
    required property int model
    required property int lineType

    property Component component: null

    // width: repNormalContent.width/repSmall.count
    // height: root.height
    color: "transparent"
    Row{
        anchors {
            top: parent.top
            left: parent.left
        }

        Repeater {
            id: repeater
            model: root.model
            Rectangle{
                width: root.width/repeater.count
                height: root.height
                color: "transparent"

                Loader{
                    sourceComponent: root.component
                }

                RuleLineEdge{
                    anchors{
                        top: parent.top
                        right: parent.right
                    }
                    lineType: root.lineType
                    value: offset+(index+1)*root.ms/repeater.count
                    visible: index !== repeater.count-1
                }
            }
        }
    }

    RuleLineEdge{
        anchors{
            top: parent.top
            right: parent.right
        }
        lineType: root.lineType
        value: root.offset+root.ms
    }
}
