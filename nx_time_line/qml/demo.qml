import QtQuick 2.15

Item{
    width: 400
    height: 300

    Rectangle {

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

    }

    signal textChanged(text: string)

    function showText(text: string) : bool {
        demoText.text = text
        return true
    }
}
