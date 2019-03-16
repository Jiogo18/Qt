import QtQuick 2.7
import QtQuick.Window 2.2
//import "Button.qml"

Window
{
    visible: true
    width: 180
    height: 420
    title: qsTr("Appli de test pour mobile")
    Column {
        Button {}
    }
    Rectangle {
        width: 10
        height: 20
        color: "red"
    }

}
