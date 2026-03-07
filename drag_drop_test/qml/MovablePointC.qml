import QtQuick

Item {
    id: root
    width: 30
    height: 30

    property string pointId: ""
    property color pointColor: "#0ea5e9"

    Drag.active: dragHandler.active
    Drag.keys: ["movable-point-c"]
    Drag.supportedActions: Qt.MoveAction

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
        }
    }
}
