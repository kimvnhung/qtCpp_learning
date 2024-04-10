import QtQuick 2.15
import models 1.0


Rectangle {
    id: root
    width: parent.width
    height: 100
    color:"transparent"

    readonly property var delegateStates : [
        [2,5,10], // 1s
        [2,3,2], // 1any
        [5,2,5], // 5s
        [5,2,3], // 5min
        [2,5,2], //10any
        [3,2,5], //30any
        [3,2,3] // 3h
    ]

    property var delegateState: delegateStates[0]


    required property int ms
    property int offset: 0

    onWidthChanged: {
        delegateItem.model = generateObjectFromArray(delegateState,0,ms,offset,width)
    }

    onDelegateStateChanged: {
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
        delegateItem.model = generateObjectFromArray(delegateState,0,ms,offset,width)
    }

    states: [
        State{
            name:"1s"
            PropertyChanges {
                target: root
                delegateState: delegateStates[0]
            }
        },
        State{
            name:"1any"
            PropertyChanges {
                target: root
                delegateState: delegateStates[1]
            }
        },
        State{
            name:"5s"
            PropertyChanges {
                target: root
                delegateState: delegateStates[2]
            }
        },
        State{
            name:"5min"
            PropertyChanges {
                target: root
                delegateState: delegateStates[3]
            }
        },
        State{
            name:"10any"
            PropertyChanges {
                target: root
                delegateState: delegateStates[4]
            }
        },
        State{
            name:"30any"
            PropertyChanges {
                target: root
                delegateState: delegateStates[5]
            }
        },
        State{
            name:"3h"
            PropertyChanges {
                target: root
                delegateState: delegateStates[6]
            }
        }
    ]
}
