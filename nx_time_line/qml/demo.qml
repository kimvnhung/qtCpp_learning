import QtQuick 2.15

Rectangle {
    width: 400
    height: 300
    color: "red"

    Text {
        id: demoText
        anchors{
            centerIn: parent
        }
        text: qsTr("HelloWorld")
        onTextChanged: {
            textChanged(demoText.text);
        }
    }

    signal textChanged(text: string)

    function showText(text: string) {
        demoText.text = text
    }
}
