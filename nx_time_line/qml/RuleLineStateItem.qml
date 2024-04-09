import QtQuick 2.15
import models 1.0


Rectangle {
    id: root
    width: parent.width
    height: 100
    color:"green"

    property int ms: 5000

    RuleLineStateDelegate{
        unit: 5000
        curCount: 1
        subCount: 5
        lineType: RuleLine.HIGHEST
        component: RuleLineStateDelegate{
            unit: 1000
            curCount: 5
            subCount: 2
            lineType: RuleLine.NORMAL
            component: RuleLineStateDelegate{
                unit: 500
                curCount: 2
                subCount: 5
                lineType: RuleLine.SMALL
                component: RuleLineStateDelegate{
                    unit: 100
                    curCount: 5
                    subCount: 0
                    lineType: RuleLine.SMALLEST
                }
            }
        }
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
