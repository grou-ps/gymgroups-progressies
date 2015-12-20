import QtQuick 2.5
import QtQuick.Window 2.2

Window {
    visible: true

    width: 300 // screenWidth
    height: 300 // screenHeight

    CameraForm {
        anchors.fill: parent
        /*mouseArea.onClicked: {
            Qt.quit();
        }*/
    }
}


