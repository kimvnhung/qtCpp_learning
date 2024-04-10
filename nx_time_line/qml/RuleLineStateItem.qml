import QtQuick 2.15
import models 1.0


Rectangle {
    id: root
    width: parent.width
    height: 100
    color:"green"

    property int ms: 5000
    property int offset: 0

    RuleLineStateDelegate{
        id: highest
        offset: parent.offset
        unit: root.ms
        curCount: 1
        subCount: 5
        lineType: RuleLine.HIGHEST
        component: RuleLineStateDelegate{
            id: normal
            unit: highest.unit/highest.subCount
            curCount: 5
            subCount: 2
            lineType: RuleLine.NORMAL
            component: RuleLineStateDelegate{
                id: small
                unit: normal.unit/normal.subCount
                curCount: 2
                subCount: 5
                lineType: RuleLine.SMALL
                component: RuleLineStateDelegate{
                    id: smallest
                    unit: small.unit/small.subCount
                    curCount: 5
                    subCount: 0
                    lineType: RuleLine.SMALLEST
                }
            }
        }
    }

    onMsChanged: {
        var secs = ms/1000
        var mins = ms/(60*1000)
        var hours = ms/(60*60*1000)
        if(secs == 1){
            state = "1sLarge"
        }else if(secs == 5){
            state = "5s"
        }else if(mins == 5){
            state = "5min"
        }else if(hours == 3){
            state = "3h"
        }else if(secs == 10 || mins == 10 || hours == 10){
            state = "10any"
        }else if(secs == 30 || mins == 30 || hours == 30){
            state = "30any"
        }else if(secs == 1 || mins == 1 || hours == 1){
            state = "1any"
        }

        console.log("state : "+state)
    }

    states: [
        State{
            name:"1sLarge"
            PropertyChanges {
                target: highest
                unit: 5000
                curCount: 1
                subCount: 5
            }
            PropertyChanges {
                target: normal
                unit: 1000
                curCount: 5
                subCount: 2
            }
            PropertyChanges {
                target: small
                unit: 500
                curCount: 2
                subCount: 5
            }
            PropertyChanges {
                target: smallest
                unit: 100
                curCount: 5
                subCount: 0
            }
        },
        State{
            name:"1s"
            PropertyChanges {
                target: rRepeater
                model: [2,5]
            }
            PropertyChanges {
                target: highest
                unit: 5000
                curCount: 1
                subCount: 5
            }
            PropertyChanges {
                target: normal
                unit: 1000
                curCount: 5
                subCount: 2
            }
            PropertyChanges {
                target: small
                unit: 500
                curCount: 2
                subCount: 5
            }
            PropertyChanges {
                target: smallest
                unit: 100
                curCount: 5
                subCount: 0
            }
        },
        State{
            name:"5s"
            // PropertyChanges {
            //     target: rRepeater
            //     model: [5,2,5]
            // }
            PropertyChanges {
                target: highest
                unit: 5000
                curCount: 1
                subCount: 5
            }
            PropertyChanges {
                target: normal
                unit: 1000
                curCount: 5
                subCount: 2
            }
            PropertyChanges {
                target: small
                unit: 500
                curCount: 2
                subCount: 5
            }
            PropertyChanges {
                target: smallest
                unit: 100
                curCount: 5
                subCount: 0
            }
        },
        State{
            name:"10any"
            PropertyChanges {
                target: rRepeater
                model: [2,5,2]
            }
            PropertyChanges {
                target: highest
                unit: 5000
                curCount: 1
                subCount: 5
            }
            PropertyChanges {
                target: normal
                unit: 1000
                curCount: 5
                subCount: 2
            }
            PropertyChanges {
                target: small
                unit: 500
                curCount: 2
                subCount: 5
            }
            PropertyChanges {
                target: smallest
                unit: 100
                curCount: 5
                subCount: 0
            }
        },
        State{
            name:"30any"
            PropertyChanges {
                target: rRepeater
                model: [3,2,5]
            }
            PropertyChanges {
                target: highest
                unit: 5000
                curCount: 1
                subCount: 5
            }
            PropertyChanges {
                target: normal
                unit: 1000
                curCount: 5
                subCount: 2
            }
            PropertyChanges {
                target: small
                unit: 500
                curCount: 2
                subCount: 5
            }
            PropertyChanges {
                target: smallest
                unit: 100
                curCount: 5
                subCount: 0
            }
        },
        State{
            name:"1any"
            PropertyChanges {
                target: rRepeater
                model: [2,3,2]
            }
            PropertyChanges {
                target: highest
                unit: 5000
                curCount: 1
                subCount: 5
            }
            PropertyChanges {
                target: normal
                unit: 1000
                curCount: 5
                subCount: 2
            }
            PropertyChanges {
                target: small
                unit: 500
                curCount: 2
                subCount: 5
            }
            PropertyChanges {
                target: smallest
                unit: 100
                curCount: 5
                subCount: 0
            }
        },
        State{
            name:"5min"
            PropertyChanges {
                target: rRepeater
                model: [5,2,3]
            }
            PropertyChanges {
                target: highest
                unit: 5000
                curCount: 1
                subCount: 5
            }
            PropertyChanges {
                target: normal
                unit: 1000
                curCount: 5
                subCount: 2
            }
            PropertyChanges {
                target: small
                unit: 500
                curCount: 2
                subCount: 5
            }
            PropertyChanges {
                target: smallest
                unit: 100
                curCount: 5
                subCount: 0
            }
        },
        State{
            name:"3h"
            PropertyChanges {
                target: rRepeater
                model: [3,2,3]
            }
            PropertyChanges {
                target: highest
                unit: 5000
                curCount: 1
                subCount: 5
            }
            PropertyChanges {
                target: normal
                unit: 1000
                curCount: 5
                subCount: 2
            }
            PropertyChanges {
                target: small
                unit: 500
                curCount: 2
                subCount: 5
            }
            PropertyChanges {
                target: smallest
                unit: 100
                curCount: 5
                subCount: 0
            }
        }

    ]
}
