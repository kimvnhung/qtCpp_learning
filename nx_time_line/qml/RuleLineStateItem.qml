import QtQuick 2.15
import models 1.0


Rectangle {
    id: root
    width: parent.width
    height: 100
    color:"green"

    property int ms: 5000

    Row{
        anchors {
            top: parent.top
            left: parent.left
        }

        Repeater{
            id: repNormal
            property int ms: root.ms/count


            model: 2
            // Rectangle{
            //     id: repNormalContent
            //     width: root.width/repNormal.count
            //     height: root.height
            //     color: "transparent"

            //     Row{
            //         anchors {
            //             top: parent.top
            //             left: parent.left
            //         }

            //         Repeater {
            //             id: repSmall
            //             property int ms: repNormal.ms/count
            //             model: 5
            //             Rectangle{
            //                 id: repSmallContent
            //                 width: repNormalContent.width/repSmall.count
            //                 height: root.height
            //                 color: "transparent"

            //                 RuleLineStateDelegate{
            //                     offset: 0
            //                     ms: 0
            //                     model: 10
            //                     lineType: RuleLine.SMALLEST
            //                 }

            //                 RuleLineEdge{
            //                     anchors{
            //                         top: parent.top
            //                         right: parent.right
            //                     }
            //                     lineType: RuleLine.SMALL
            //                     value: (index+1)*repSmall.ms+(repNormal.index+1)*repNormal.ms
            //                     visible: index !== repSmall.count-1
            //                 }
            //             }
            //         }
            //     }

            //     RuleLineEdge{
            //         anchors{
            //             top: parent.top
            //             right: parent.right
            //         }
            //         lineType: RuleLine.NORMAL
            //         value: (index+1)*repNormal.ms
            //         visible: index !== repNormal.count-1
            //     }
            // }
            RuleLineStateDelegate{
                offset: 0
                ms: 1000
                model: 2
                lineType: RuleLine.NORMAL
            }
        }
    }

    RuleLineEdge{
        anchors{
            top: parent.top
            right: parent.right
        }
        lineType: RuleLine.HIGHEST
        value: root.ms
    }

    states: [
        State{
            name:"1sLarge"
            PropertyChanges {
                target: repNormal
                model: 2
            }
            PropertyChanges {
                target: repSmall
                model: 5
            }
            PropertyChanges {
                target: repSmallest
                model: 10
            }
        }
        // State{
        //     name:"1s"
        //     PropertyChanges {
        //         target: rRepeater
        //         model: [2,5]
        //     }
        // },
        // State{
        //     name:"5s"
        //     PropertyChanges {
        //         target: rRepeater
        //         model: [5,2,5]
        //     }
        // },
        // State{
        //     name:"10any"
        //     PropertyChanges {
        //         target: rRepeater
        //         model: [2,5,2]
        //     }
        // },
        // State{
        //     name:"30any"
        //     PropertyChanges {
        //         target: rRepeater
        //         model: [3,2,5]
        //     }
        // },
        // State{
        //     name:"1any"
        //     PropertyChanges {
        //         target: rRepeater
        //         model: [2,3,2]
        //     }
        // },
        // State{
        //     name:"5min"
        //     PropertyChanges {
        //         target: rRepeater
        //         model: [5,2,3]
        //     }
        // },
        // State{
        //     name:"3h"
        //     PropertyChanges {
        //         target: rRepeater
        //         model: [3,2,3]
        //     }
        // }

    ]
}
