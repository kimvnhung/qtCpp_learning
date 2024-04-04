import QtQuick 2.15

Item {
    Column{
        Rectangle{
            width: 100
            height: 50
            color: "red"
            Text {
                text: "hello"
            }
        }
    }

    ScrollableView{
        width: 300
        contentItem: Rectangle{
            width: 100
            height: 50
            color: "red"
            Text {
                text: "hello"
            }
        }

    }
}
