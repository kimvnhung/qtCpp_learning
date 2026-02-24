import QtQuick
import QtQuick.Controls
import TranslationLib

ApplicationWindow {
    visible: true
    width: 420
    height: 260

    title: qsTr("Qt Translation Demo")

    Component.onCompleted: console.log("[i18n] qsTrId(app.qml_hello_id):", qsTrId("app.qml_hello_id"))

    Backend { id: backend }

    Column {
        anchors.centerIn: parent
        spacing: 10

        Label {
            id: cxxTrLabel
            text: backend.cxxTrExample()
        }
        Label {
            id: cxxIdLabel
            text: backend.cxxIdExample()
        }
        Label { text: qsTr("Hello from QML qsTr()") }
        //% "Hello from QML qsTrId()"
        Label { text: qsTrId("app.qml_hello_id") }

        Button {
            text: langManager === null ? qsTr("Initializing...") : langManager.language === "vi" ? qsTr("Switch to English") : qsTr("Switch to Vietnamese")
            onClicked: langManager.toggleLanguage()
        }
    }

    Connections {
        target: langManager
        function onLanguageChanged() {
            cxxTrLabel.text = backend.cxxTrExample()
            cxxIdLabel.text = backend.cxxIdExample()
        }
    }
}
