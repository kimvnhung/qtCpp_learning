import QtQuick
import QtQuick.Controls

Item {
	id: root

	property string componentId: ""
	property real componentX: 0
	property real componentY: 0
	property var pointsModel: []

	signal pointMoveRequested(string sourceComponentId, string pointId, real dropX, real dropY)

	Rectangle {
		id: frame
		anchors.fill: parent
		radius: 12
		color: "#ffffff"
		border.width: 2
		border.color: dropZone.containsDrag ? "#0891b2" : "#94a3b8"
	}

	Rectangle {
		id: titleBar
		x: 0
		y: 0
		width: parent.width
		height: 42
		radius: 12
		color: "#e2e8f0"

		Rectangle {
			x: 0
			y: parent.height - 12
			width: parent.width
			height: 12
			color: parent.color
		}

		Text {
			anchors.verticalCenter: parent.verticalCenter
			anchors.left: parent.left
			anchors.leftMargin: 12
			color: "#0f172a"
			font.pixelSize: 14
			font.bold: true
			text: root.componentId
		}

		Text {
			anchors.verticalCenter: parent.verticalCenter
			anchors.right: parent.right
			anchors.rightMargin: 12
			color: "#334155"
			font.pixelSize: 12
			text: "x=" + Math.round(root.componentX)
				  + " y=" + Math.round(root.componentY)
				  + " size=" + Math.round(root.width) + "x" + Math.round(root.height)
		}
	}

	Item {
		id: pointArea
		x: 8
		y: titleBar.height + 8
		width: parent.width - 16
		height: parent.height - titleBar.height - 16
		clip: false

		DropArea {
			id: dropZone
			anchors.fill: parent
			keys: ["movable-point"]

			onDropped: function(drop) {
				if (!drop.source || !drop.source.pointId || !drop.source.componentId) {
					return
				}

				var targetX = Math.max(0, Math.min(pointArea.width - 20, drop.x - 10))
				var targetY = Math.max(0, Math.min(pointArea.height - 20, drop.y - 10))

				drop.source.dropAccepted = true
				root.pointMoveRequested(drop.source.componentId, drop.source.pointId, targetX, targetY)
				drop.acceptProposedAction()
			}
		}

		Repeater {
			model: root.pointsModel

			delegate: MovablePoint {
				pointId: modelData.id
				componentId: root.componentId
				pointColor: modelData.color
				x: modelData.x
				y: modelData.y
			}
		}
	}

}
