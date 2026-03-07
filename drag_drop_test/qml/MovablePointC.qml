import QtQuick

Item {
    id: root
    width: 30
    height: 30

    property string pointId: ""
    property color pointColor: "#0ea5e9"

    Drag.active: dragHandler.active
    Drag.hotSpot.x: width / 2
    Drag.hotSpot.y: height / 2
    Drag.supportedActions: Qt.MoveAction
    Drag.keys: [ "movable-point-c" ]

    Rectangle {
        anchors.fill: parent
        radius: width / 2
        color: root.pointColor
        border.color: "#0f172a"
        border.width: 1
    }

    DragHandler {
        id: dragHandler
        target: root
        onActiveChanged: {
            console.log("onActiveDragHandler: ",pointId, dragHandler.active)
            if(dragHandler.active)
            {
                console.log("Drag start with source: ", root.Drag.source,"taget: ", root.Drag.target)
            }

            if(!dragHandler.active && root.Drag.target !== null){
                console.log("Drag start with source: ", root.Drag.source,"taget: ", root.Drag.target)
                root.parent = root.Drag.target
                root.x = root.parent.width / 2 - root.width / 2
                root.y = root.parent.height / 2 - root.height / 2
            }
        }
    }
}
