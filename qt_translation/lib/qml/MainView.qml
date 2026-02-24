import QtQuick
import QtQuick.Controls
import TranslationLib

ApplicationWindow {
    visible: true
    width: 420
    height: 260

    title: qsTr("Qt Translation Demo")

    Backend { id: backend }

    Column {
        anchors.centerIn: parent
        spacing: 10

        Label { text: backend.cxxTrExample() }
        Label { text: backend.cxxIdExample() }
        Label { text: qsTr("Hello from QML qsTr()") }
        //% "Hello from QML qsTrId()"
        Label { text: qsTrId("app.qml_hello_id") }

        Button {
            text: langManager === null ? qsTr("Initializing...") : langManager.language === "vi" ? qsTr("Switch to English") : qsTr("Switch to Vietnamese")
            onClicked: langManager.toggleLanguage()
        }
    }
}
