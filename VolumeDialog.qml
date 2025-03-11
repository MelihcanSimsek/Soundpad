import QtQuick
import QtQuick.Controls
import com.company.SoundpadController
import QtQuick.Controls.Material
import QtQuick.Dialogs

Dialog {
  id: root
  title: "Audio Volume Settings"
  closePolicy: "NoAutoClose"
  modal: true
  anchors.centerIn: parent
  width: parent.width * 0.85
  height: parent.height * 0.7
  Material.elevation: 24
  Connections {
      target: SoundpadController
      function onVolumeSliderChanged(outputVolume, inputVolume) {
          outputVolumeSlider.value = outputVolume;
          inputVolumeSlider.value = inputVolume;
      }
  }

  background: Rectangle {
    color: Material.dialogColor
    radius: 16
    border.color: Material.accent
    border.width: 2
  }

  contentItem: Item {
    anchors.fill: parent

    Column {
      anchors.fill: parent
      anchors.margins: 24
      spacing: 24

      Item {
        width: parent.width
        height: 20
      }

      Column {
        width: parent.width
        spacing: 16

        Label {
          text: "Output Audio Volume"
          font.pixelSize: 18
          font.weight: Font.Medium
          color: Material.foreground
        }

        Row {
          width: parent.width
          spacing: 12

          Rectangle {
            width: 32
            height: 32
            radius: 16
            color: "transparent"

            Text {
              anchors.centerIn: parent
              text: outputVolumeSlider.value < 5 ? "🔇" :
                                                   outputVolumeSlider.value < 50 ? "🔉" : "🔊"
              font.pixelSize: 18
            }
          }

          Slider {
            id: outputVolumeSlider
            width: parent.width - 92
            from: 0
            to: 100
            stepSize: 1

            background: Rectangle {
              x: outputVolumeSlider.leftPadding
              y: outputVolumeSlider.topPadding + outputVolumeSlider.availableHeight / 2 - height / 2
              width: outputVolumeSlider.availableWidth
              height: 6
              radius: 3
              color: "#555555"

              Rectangle {
                width: outputVolumeSlider.visualPosition * parent.width
                height: parent.height
                color: Material.accent
                radius: 3
              }
            }

            handle: Rectangle {
              x: outputVolumeSlider.leftPadding + outputVolumeSlider.visualPosition * (outputVolumeSlider.availableWidth - width)
              y: outputVolumeSlider.topPadding + outputVolumeSlider.availableHeight / 2 - height / 2
              width: 20
              height: 20
              radius: 10
              color: outputVolumeSlider.pressed ? Qt.darker(Material.accent, 1.1) : Material.accent
              border.color: "#ffffff"
              border.width: 2
            }
          }

          Label {
            width: 48
            text: Math.round(outputVolumeSlider.value) + "%"
            font.pixelSize: 16
            horizontalAlignment: Text.AlignRight
            color: Material.foreground
          }
        }
      }

      Column {
        width: parent.width
        spacing: 16

        Label {
          text: "Input Audio Volume"
          font.pixelSize: 18
          font.weight: Font.Medium
          color: Material.foreground
        }

        Row {
          width: parent.width
          spacing: 12

          Rectangle {
            width: 32
            height: 32
            radius: 16
            color: "transparent"

            Text {
              anchors.centerIn: parent
              text: inputVolumeSlider.value < 5 ? "🎤✕" : "🎤"
              font.pixelSize: 18
              color:Material.foreground
            }
          }

          Slider {
            id: inputVolumeSlider
            width: parent.width - 92
            from: 0
            to: 100
            stepSize: 1

            background: Rectangle {
              x: inputVolumeSlider.leftPadding
              y: inputVolumeSlider.topPadding + inputVolumeSlider.availableHeight / 2 - height / 2
              width: inputVolumeSlider.availableWidth
              height: 6
              radius: 3
              color: "#555555"

              Rectangle {
                width: inputVolumeSlider.visualPosition * parent.width
                height: parent.height
                color: Material.accent
                radius: 3
              }
            }

            handle: Rectangle {
              x: inputVolumeSlider.leftPadding + inputVolumeSlider.visualPosition * (inputVolumeSlider.availableWidth - width)
              y: inputVolumeSlider.topPadding + inputVolumeSlider.availableHeight / 2 - height / 2
              width: 20
              height: 20
              radius: 10
              color: inputVolumeSlider.pressed ? Qt.darker(Material.accent, 1.1) : Material.accent
              border.color: "#ffffff"
              border.width: 2
            }
          }

          Label {
            width: 48
            text: Math.round(inputVolumeSlider.value) + "%"
            font.pixelSize: 16
            horizontalAlignment: Text.AlignRight
            color: Material.foreground
          }
        }

        Item {
          width: parent.width
          height: 28
          visible: inputVolumeSlider.value > -1

          Rectangle {
            anchors.fill: parent
            radius: 4
            color: Qt.rgba(255, 255, 255, 0.1)
          }

          Row {
            anchors.fill: parent
            anchors.margins: 4
            spacing: 2

            Repeater {
              model: 20

              Rectangle {
                width: (parent.width - 38) / 20
                height: 20
                radius: 2

                property real threshold: index / 20
                property bool active: inputVolumeSlider.value / 100 > threshold

                color: active ?
                         (threshold > 0.8 ? "#ff5252" :
                                            threshold > 0.6 ? "#ffab40" :
                                                              Material.accent) :
                         Qt.rgba(0, 0, 0, 0.1)

                Behavior on color {
                  ColorAnimation { duration: 50 }
                }
              }
            }
          }
        }
      }

      Item {
        width: parent.width
        height: 20
      }

      Row {
        width: parent.width
        spacing: 12
        layoutDirection: Qt.RightToLeft

        Button {
          text: "Save"
          width: (parent.width - 12) / 2
          height: 48
          Material.background: Material.accent
          Material.foreground: "#ffffff"
          font.bold: true
          font.pixelSize: 16

          background: Rectangle {
            color: parent.down ? Qt.darker(Material.accent, 1.2) :
                                 parent.hovered ? Qt.lighter(Material.accent, 1.1) :
                                                  Material.accent
            radius: 8
          }

          onClicked: {
            root.close()
            SoundpadController.changeOutputVolume(outputVolumeSlider.value)
            SoundpadController.updateVolume(outputVolumeSlider.value,inputVolumeSlider.value)
          }
        }
      }
    }
  }
}
