import QtQuick
import QtQuick.Controls

Item {
	id: root

	property int gridStep: 40
	property var componentsData: [
		{
			id: "Component-A",
			x: 120,
			y: 120,
			width: 320,
			height: 250,
			points: [
				{ id: "A-1", x: 36, y: 46, color: "#f97316" },
				{ id: "A-2", x: 120, y: 90, color: "#10b981" },
				{ id: "A-3", x: 210, y: 152, color: "#3b82f6" }
			]
		},
		{
			id: "Component-B",
			x: 560,
			y: 260,
			width: 320,
			height: 250,
			points: [
				{ id: "B-1", x: 56, y: 62, color: "#f43f5e" },
				{ id: "B-2", x: 170, y: 132, color: "#8b5cf6" }
			]
		}
	]

	function clonePoint(point) {
		return {
			id: point.id,
			x: point.x,
			y: point.y,
			color: point.color
		}
	}

	function movePoint(sourceComponentId, pointId, targetComponentId, targetX, targetY) {
		var nextData = []
		var movingPoint = null

		for (var i = 0; i < componentsData.length; ++i) {
			var component = componentsData[i]
			var nextComponent = {
				id: component.id,
				x: component.x,
				y: component.y,
				width: component.width,
				height: component.height,
				points: []
			}

			for (var j = 0; j < component.points.length; ++j) {
				var point = component.points[j]
				if (component.id === sourceComponentId && point.id === pointId) {
					movingPoint = clonePoint(point)
					continue
				}
				nextComponent.points.push(clonePoint(point))
			}

			nextData.push(nextComponent)
		}

		if (!movingPoint) {
			return
		}

		movingPoint.x = targetX
		movingPoint.y = targetY

		for (var k = 0; k < nextData.length; ++k) {
			if (nextData[k].id === targetComponentId) {
				nextData[k].points.push(movingPoint)
				componentsData = nextData
				return
			}
		}
	}

	Rectangle {
		anchors.fill: parent
		color: "#f5f7fb"
	}

	Canvas {
		id: gridCanvas
		anchors.fill: parent

		onPaint: {
			var ctx = getContext("2d")
			ctx.reset()

			ctx.strokeStyle = "#dce2ea"
			ctx.lineWidth = 1

			for (var x = 0; x <= width; x += root.gridStep) {
				ctx.beginPath()
				ctx.moveTo(x, 0)
				ctx.lineTo(x, height)
				ctx.stroke()
			}

			for (var y = 0; y <= height; y += root.gridStep) {
				ctx.beginPath()
				ctx.moveTo(0, y)
				ctx.lineTo(width, y)
				ctx.stroke()
			}

			ctx.strokeStyle = "#8f99ab"
			ctx.lineWidth = 2

			ctx.beginPath()
			ctx.moveTo(0, 0)
			ctx.lineTo(width, 0)
			ctx.stroke()

			ctx.beginPath()
			ctx.moveTo(0, 0)
			ctx.lineTo(0, height)
			ctx.stroke()
		}

		onWidthChanged: requestPaint()
		onHeightChanged: requestPaint()
	}

	Repeater {
		model: Math.floor(root.width / root.gridStep) + 1
		delegate: Text {
			x: index * root.gridStep + 4
			y: 4
			color: "#6b7280"
			text: index * root.gridStep
			font.pixelSize: 10
		}
	}

	Repeater {
		model: Math.floor(root.height / root.gridStep) + 1
		delegate: Text {
			x: 4
			y: index * root.gridStep + 4
			color: "#6b7280"
			text: index * root.gridStep
			font.pixelSize: 10
		}
	}

	Repeater {
		model: root.componentsData

		delegate: ComponentItem {
			x: modelData.x
			y: modelData.y
			width: modelData.width
			height: modelData.height
			componentId: modelData.id
			componentX: x
			componentY: y
			pointsModel: modelData.points

			onPointMoveRequested: function(sourceComponentId, pointId, dropX, dropY) {
				root.movePoint(sourceComponentId, pointId, componentId, dropX, dropY)
			}
		}
	}

}
