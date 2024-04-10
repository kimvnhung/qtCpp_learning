import QtQuick 2.15
import models 1.0


Rectangle {
    id: root
    width: parent.width
    height: 100
    color:"transparent"

    readonly property var delegateStates : [
        [2,5,10]
    ]

    property var delegateState: delegateStates[0]


    required property int ms
    property int offset: 0

    onWidthChanged: {
        delegateItem.model = generateObjectFromArray(delegateState,0,ms,offset,width)
    }

    function generateObjectFromArray(array,depth,value,offset, width) {
        var children = []
        var curChildCount = array[depth]

        for (var i = 0;i < curChildCount;i++){
            if(depth === array.length-1){
                children.push({
                              'type': 'leaf',
                              'value': (offset+(i+1)*value/curChildCount),
                              'width': width/curChildCount,
                              'lineType': depth+1,
                              })
            }else {
                console.log("nextObset "+(offset+(i+1)*value/curChildCount))
                if(i === curChildCount-1){
                    children.push({
                                  'type': 'node',
                                  'value': (offset+(i+1)*value/curChildCount),
                                  'width': width/curChildCount,
                                  'lineType': RuleLine.SMALLEST,
                                  'children': (depth+1 < array.length) ? generateObjectFromArray(array,depth+1,value/curChildCount,(offset +i*value/curChildCount),width/curChildCount) : []
                                  })
                }else {
                    children.push({
                                  'type': 'node',
                                  'value': (offset+(i+1)*value/curChildCount),
                                  'width': width/curChildCount,
                                  'lineType': (depth+1),
                                  'children': (depth+1 < array.length) ? generateObjectFromArray(array,depth+1,value/curChildCount,(offset +i*value/curChildCount),width/curChildCount) : []
                                  })
                }
            }
        }
        return children
    }


    RuleLineStateDelegate2{
        id: delegateItem
        width: parent.width
        height: parent.height
        unit: ms
        offset: root.offset
        lineType: RuleLine.HIGHEST
        delegate: RuleStateDelegateChooser{
            height: parent.height
        }
        model: []
    }

    // onMsChanged: {
    //     // var secs = ms/1000
    //     // var mins = ms/(60*1000)
    //     // var hours = ms/(60*60*1000)
    //     // if(secs == 1){
    //     //     state = "1sLarge"
    //     // }else if(secs == 5){
    //     //     state = "5s"
    //     // }else if(mins == 5){
    //     //     state = "5min"
    //     // }else if(hours == 3){
    //     //     state = "3h"
    //     // }else if(secs == 10 || mins == 10 || hours == 10){
    //     //     state = "10any"
    //     // }else if(secs == 30 || mins == 30 || hours == 30){
    //     //     state = "30any"
    //     // }else if(secs == 1 || mins == 1 || hours == 1){
    //     //     state = "1any"
    //     // }

    //     // console.log("state : "+state)
    // }

    states: [
        State{
            name:"1sLarge"
            PropertyChanges {
                target: highest
                unit: 1000
                subCount: 2
            }
            PropertyChanges {
                target: normal
                unit: 500
                subCount: 5
            }
            PropertyChanges {
                target: small
                unit: 100
                subCount: 10
            }
            PropertyChanges {
                target: smallest
                unit: 10
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
