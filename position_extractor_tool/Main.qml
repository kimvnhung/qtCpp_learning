import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls
import QtQuick.Dialogs

import app 1.0

Window {
    width: 800
    height: 600
    visible: true
    title: "Image Rectangle Tool"

    property bool drawing: false
    property real startX: 0
    property real startY: 0
    property real rectX: 0
    property real rectY: 0
    property real rectW: 0
    property real rectH: 0
    property string mousePos: ""

    FileDialog {
        id: fileDialog
        title: "Select an image"
        nameFilters: ["Images (*.png *.jpg *.jpeg *.bmp)"]
        currentFolder: appModel.imageFolder
        onAccepted: {
            appModel.imagePath = selectedFile
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "#2b2b2b"

        Image {
            id: img
            anchors.fill: parent
            source: appModel.imagePath
            // smooth: true
            // Fit width while maintaining aspect ratio
            fillMode: Image.PreserveAspectFit

            MouseArea {
                function isLandscape() {
                    return appModel.imageSize.width > appModel.imageSize.height;
                }

                function imageRatio() {
                    return appModel.imageSize.width / appModel.imageSize.height;
                }

                function getWidth() {
                    return isLandscape() ? parent.width : (parent.height * imageRatio());
                }

                function getHeight() {
                    return isLandscape() ? (parent.width / imageRatio()) : parent.height;
                }

                width: getWidth()
                height: getHeight()
                anchors {
                    centerIn: parent
                }

                Component.onCompleted: {
                    console.log("size: ", appModel.imageSize.width, "x", appModel.imageSize.height);
                }

                hoverEnabled: true

                onPositionChanged: function(mouse) {
                    mousePos = `Mouse: ${Math.round(mouse.x)}, ${Math.round(mouse.y)}`
                    if (drawing) {
                        rectW = mouse.x - startX
                        rectH = mouse.y - startY
                    }
                }

                onPressed: {
                    startX = mouse.x
                    startY = mouse.y
                    rectX = startX
                    rectY = startY
                    rectW = 0
                    rectH = 0
                    drawing = true
                }

                onReleased: {
                    drawing = false
                    let info = `X:${Math.round(rectX)}, Y:${Math.round(rectY)}, W:${Math.round(rectW)}, H:${Math.round(rectH)}`
                    console.log("Rectangle:", info)
                }

                Rectangle {
                    id: selectionRect
                    x: rectX
                    y: rectY
                    width: rectW
                    height: rectH
                    color: "transparent"
                    border.color: "red"
                    visible: rectW !== 0 && rectH !== 0
                }
            }


        }

        Text {
            text: mousePos
            color: "white"
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 8
            font.pixelSize: 14
        }

        Row {
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.margins: 8
            spacing: 8

            Button {
                text: "Open Image"
                onClicked: fileDialog.open()
            }
        }
    }
}
