import QtQuick

Image {
    id: root
    property alias baseImage: root.source
    mipmap: true
    fillMode: Image.PreserveAspectFit

    signal clicked()

    opacity: buttonMouseArea.containsMouse ? 0.6 : 1

    MouseArea {
        id: buttonMouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: buttonMouseArea.containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor

        onClicked: {
            root.clicked();
        }
    }
}
