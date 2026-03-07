import QtQuick

Item {
	id: root
	width: 20
	height: 20

	property string pointId: ""
	property string componentId: ""
	property color pointColor: "#0ea5e9"

	property bool dropAccepted: false
	property real startX: 0
	property real startY: 0

	Drag.active: dragHandler.active
	Drag.keys: ["movable-point"]
	Drag.hotSpot.x: width / 2
	Drag.hotSpot.y: height / 2
	Drag.supportedActions: Qt.MoveAction

	Rectangle {
		anchors.fill: parent
		radius: width / 2
		color: root.pointColor
		border.color: "#0f172a"
		border.width: dragHandler.active ? 2 : 1
		opacity: dragHandler.active ? 0.8 : 1.0
	}

	Text {
		anchors.centerIn: parent
		color: "#ffffff"
		font.bold: true
		font.pixelSize: 10
		text: root.pointId.slice(-1)
	}

	DragHandler {
		id: dragHandler
		target: root

		onActiveChanged: {
			if (active) {
				root.startX = root.x
				root.startY = root.y
				root.dropAccepted = false
				return
			}

			if (!root.dropAccepted) {
				root.x = root.startX
				root.y = root.startY
			}
		}
	}

}
