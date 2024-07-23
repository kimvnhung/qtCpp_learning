import QtQuick 2.15

Item {
    id: root
    Column{
        Repeater{
            model: instance.menuItems
            Rectangle{
                id: item
                height: root.width/instance.menuItems.length
                width: root.width
                MouseArea{
                    width: parent.width
                    height: parent.height
                    hoverEnabled: true
                    onPressed: {
                        instance.onMenuItemClicked(index)
                    }
                    onEntered: {
                        item.color = "red"
                    }
                    onExited: {
                        item.color = "white"
                    }
                }

                Text {
                    text: modelData
                    anchors.centerIn: parent
                }
            }
        }
    }
}
