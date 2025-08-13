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
                id: mouseArea
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

                function toImageX(cordinateX) {
                    return ((cordinateX / width) * appModel.imageSize.width).toFixed(0);
                }

                function toImageY(cordinateY) {
                    return ((cordinateY / height) * appModel.imageSize.height).toFixed(0);
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
                    if (drawing) {
                        rectW = mouse.x - startX
                        rectH = mouse.y - startY
                    }

                    if(!appModel.isInfoSaved){
                        if(selectionRect.visible) {
                            appModel.infoText = "("+ toImageX(mouseX) + ", " + toImageY(mouseY) + "," + toImageX(rectW) + ", " + toImageY(rectH) + ")";
                        }else {
                            appModel.infoText = "("+ toImageX(mouseX) + ", " + toImageY(mouseY) + ")";
                        }
                    }

                    console.log("mouseArea.x, mouseX, infoTips.width, img.width:", mouseArea.x, mouseX, infoTips.width, img.width);
                    appModel.infoRectX = (mouseArea.x + mouseX + infoTips.width) > img.width ? (img.width - infoTips.width) : (mouseArea.x+mouseX)
                    appModel.infoRectY = (mouseArea.y + mouseY - 20 + infoTips.height) > img.height ? (img.height - infoTips.height) : (mouseArea.y+mouseY - 20)
                }

                onPressed: function(mouse) {
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
                    if(rectW > 1 && rectH > 1) {
                        appModel.saveRect()
                    }
                }

                Rectangle {
                    id: selectionRect
                    x: rectX
                    y: rectY
                    width: rectW
                    height: rectH
                    color: "transparent"
                    border.color: "red"
                    border.width: 2
                    visible: rectW > 1 && rectH > 1
                    onVisibleChanged: {
                        if(!visible)
                            appModel.isInfoSaved = false;
                    }
                }
            }


        }

        Rectangle {
            id: infoTips
            width: appModel.infoRectWidth
            height: 18
            color: "white"
            x: appModel.infoRectX
            y: appModel.infoRectY
            Text {
                anchors.fill: parent
                color: "black"
                horizontalAlignment: Text.AlignHCenter
                text: appModel.infoText
                font {
                    weight: Font.Bold
                }
            }
        }

        Text {
            anchors {
                bottom: parent.bottom
                bottomMargin: 50
            }
            anchors.horizontalCenter: parent.horizontalCenter

            color: "green"
            font {
                pixelSize: 20
                weight: Font.Bold
            }

            text: appModel.infoText +" saved to clipboard!"
            visible: appModel.isInfoSaved
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
