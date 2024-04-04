import QtQuick 2.15

Item {
    property Component contentItem : null

    Column{
        spacing: 5
        Loader {
            width: contentItem.width
            height: contentItem.height
            sourceComponent: contentItem
        }

        ScrollThumbnail{
            width: parent.width
        }
    }
}
