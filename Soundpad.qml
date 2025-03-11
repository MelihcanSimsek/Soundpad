import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import com.company.SoundpadController

Rectangle {
  id: root
  property alias backgroundColor: root.color
  property color foregroundColor: root.color
  color: "#121212"

  property color primaryColor: "#6200EA"    // Deep purple
  property color accentColor: "#00E676"     // Green accent
  property color recordingColor: "#F44336"   // Red for recording
  property color playingColor: "#00C853"     // Green for playing
  property color inactiveColor: "#757575"    // Gray for disabled
  property color sliderBackground: "#263238" // Dark blue-gray

  GridView {
    id: gridView
    anchors {
      fill: parent
      leftMargin: 20
      topMargin: 20
      bottomMargin: 20
    }
    cellWidth: 110
    cellHeight: 110
    model: SoundpadController
    clip: true

    ScrollBar.vertical: ScrollBar {
      active: true
      policy: ScrollBar.AsNeeded
      Material.accent: accentColor
    }

    delegate: Item {
      id: delegate
      width: gridView.cellWidth - 10
      height: gridView.cellHeight - 10

      required property string soundId
      required property string soundName
      required property url imagePath
      required property url soundPath
      required property string soundColor
      required property int index

      Rectangle {
        id: shadowRect
        anchors.centerIn: parent
        width: buttonArea.width + 4
        height: buttonArea.height + 4
        radius: buttonArea.radius + 2
        color: "#40000000"  // Semi-transparent shadow
        visible: true

        scale: soundMouseArea.containsMouse ? 1.03 : 1.0
        Behavior on scale { NumberAnimation { duration: 200 } }
      }

      Rectangle {
        id: buttonArea
        anchors.centerIn: parent
        width: parent.width
        height: parent.height
        color: delegate.soundColor
        radius: 12

        Rectangle {
          id: innerHighlight
          anchors {
            left: parent.left
            right: parent.right
            top: parent.top
          }
          height: parent.height / 2
          radius: parent.radius
          color: Qt.lighter(parent.color, 1.2)
          opacity: 0.3
        }

        scale: soundMouseArea.pressed ? 0.95 : 1.0
        Behavior on scale { NumberAnimation { duration: 100; easing.type: Easing.OutQuad } }

        states: [
          State {
            name: "hovered"
            when: soundMouseArea.containsMouse
            PropertyChanges { target: buttonArea; color: Qt.lighter(delegate.soundColor, 1.1) }
            PropertyChanges { target: soundImage; scale: 1.1 }
            PropertyChanges { target: soundText; color: foregroundColor; font.pixelSize: 17 }
          }
        ]

        transitions: [
          Transition {
            from: ""; to: "hovered"; reversible: true
            ParallelAnimation {
              ColorAnimation { duration: 200 }
              NumberAnimation { properties: "scale"; duration: 200; easing.type: Easing.OutQuad }
              ColorAnimation { properties: "color"; duration: 200 }
              NumberAnimation { properties: "font.pixelSize"; duration: 200 }
            }
          }
        ]

        Image {
          id: soundImage
          anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            topMargin: 15
          }
          source: delegate.imagePath
          width: 45
          height: 45
          mipmap: true
          fillMode: Image.PreserveAspectFit
          opacity: 0.9

          RotationAnimation {
            id: rotationAnimation
            target: soundImage
            from: 0
            to: 360
            duration: 500
            running: false
          }

          Behavior on scale { NumberAnimation { duration: 200; easing.type: Easing.OutBack } }
        }

        Text {
          id: soundText
          anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
            bottomMargin: 15
          }
          text: delegate.soundName
          color: foregroundColor
          font {
            pixelSize: 16
            bold: true
            family: "Arial"
          }

          opacity: soundMouseArea.containsMouse ? 1.0 : 0.9
          Behavior on opacity { NumberAnimation { duration: 200 } }
        }

        Rectangle {
          id: playingIndicator
          width: 8
          height: 8
          radius: 4
          color: playingColor
          anchors {
            right: parent.right
            top: parent.top
            margins: 8
          }
          visible: !!SoundpadController.currentSound && SoundpadController.currentSound.id === delegate.soundId
          SequentialAnimation {
            id: pulseAnimation
            running: playingIndicator.visible
            loops: Animation.Infinite

            NumberAnimation {
              target: playingIndicator
              property: "opacity"
              from: 1.0
              to: 0.5
              duration: 800
              easing.type: Easing.InOutQuad
            }

            NumberAnimation {
              target: playingIndicator
              property: "opacity"
              from: 0.5
              to: 1.0
              duration: 800
              easing.type: Easing.InOutQuad
            }
          }
        }
      }

      MouseArea {
        id: soundMouseArea
        anchors.fill: buttonArea
        hoverEnabled: true
        onClicked: {
          SoundpadController.setCurrentSound(
                delegate.soundId,
                delegate.soundName,
                delegate.imagePath,
                delegate.soundPath,
                delegate.soundColor
                )

          clickAnimation.start()
          rotationAnimation.start()
        }
        onPressed: {
          buttonArea.color = Qt.darker(delegate.soundColor, 1.1)
        }
        onReleased: {
          if (containsMouse) {
            buttonArea.color = Qt.lighter(delegate.soundColor, 1.1)
          } else {
            buttonArea.color = delegate.soundColor
          }
        }
        cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
      }

      Item {
        id: rippleContainer
        anchors.fill: buttonArea
        clip: true

        Rectangle {
          id: ripple
          property real size: 0

          width: size
          height: size
          radius: size/2
          x: rippleContainer.width/2 - size/2
          y: rippleContainer.height/2 - size/2
          color: "#80FFFFFF"
          opacity: 0
        }
      }

      ParallelAnimation {
        id: clickAnimation

        NumberAnimation {
          target: ripple
          property: "size"
          from: 0
          to: delegate.width * 2
          duration: 400
          easing.type: Easing.OutQuad
        }

        NumberAnimation {
          target: ripple
          property: "opacity"
          from: 0.7
          to: 0
          duration: 400
          easing.type: Easing.OutQuad
        }
      }


      Rectangle{
          id:deleteButton
          visible: false
          enabled: false
          anchors{
            centerIn: parent.Center
          }
          width: gridView.cellWidth - 10
          height: gridView.cellWidth - 10
          radius: 12

          Connections {
            target: SoundpadController
            function onDeletingModeChanged(mode) {
              deleteButton.visible = mode
              deleteButton.enabled = mode
            }
          }

          color:if(deleteButtonMouseArea.containsPress)
                {
                  return Qt.darker("white")
                } else if(deleteButtonMouseArea.containsMouse){
                  return backgroundColor
                } else{
                  return "white"
                }

          Image{
            anchors.centerIn: parent
            fillMode: Image.PreserveAspectFit
            source:"qrc:/assets/icons/delete.png"
            width: parent.width
            height:parent.height
          }

          MouseArea{
            id:deleteButtonMouseArea
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: deleteButtonMouseArea.containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
            onClicked: {
              SoundpadController.deleteSound(delegate.soundId,delegate.imagePath,delegate.soundPath)
            }
          }
        }
    }
  }
}
