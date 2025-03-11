import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Dialogs
import com.company.SoundpadController



Dialog {
  id: addSoundDialog

  property color soundButton1: "#7C4DFF"
  property color soundButton2: "#3D5AFE"
  property color soundButton3: "#2979FF"
  property color soundButton4: "#00B0FF"
  property color soundButton5: "#00E5FF"
  property color soundButton6: "#1DE9B6"

  title: "Add New Sound"
  modal: true
  closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
  anchors.centerIn: parent
  width: parent.width*0.85
  height: parent.height*0.95

  Material.elevation: 24

  background: Rectangle {
    color: Material.dialogColor
    radius: 10
    border.color: Material.accent
    border.width: 2
  }


  contentItem: Item {
    anchors.fill: parent

    Column {
      anchors.fill: parent
      anchors.margins: 20
      spacing: 24

      // Header with title
      Label {
        text: "Add New Sound"
        font.pixelSize: 24
        font.weight: Font.Bold
        width: parent.width
        color: Material.foreground
      }

      // Sound name input
      Column {
        width: parent.width
        spacing: 8

        Label {
          text: "Sound Name"
          font.pixelSize: 16
          color: Material.foreground
        }

        TextField {
          id: soundNameField
          width: parent.width
          placeholderText: "Enter sound name"
          selectByMouse: true
          color: Material.foreground

          background: Rectangle {
            color: Material.background
            radius: 5
            border.color: soundNameField.activeFocus ? Material.accent : "#555555"
            border.width: soundNameField.activeFocus ? 2 : 1
          }
        }
      }

      // Image selector
      Column {
        width: parent.width
        spacing: 8

        Label {
          text: "Sound Image"
          font.pixelSize: 16
          color: Material.foreground
        }

        Row {
          width: parent.width
          spacing: 10

          TextField {
            id: imagePathField
            width: parent.width - imageButton.width - 10
            readOnly: true
            placeholderText: "No image selected"
            color: Material.foreground

            background: Rectangle {
              color: Material.background
              radius: 5
              border.color: "#555555"
              border.width: 1
            }
          }

          Button {
            id: imageButton
            text: "Browse"
            Material.background: Material.primary
            Material.foreground: "#FFFFFF"

            onClicked: imageFileDialog.open()
          }
        }

        // Preview of selected image
        Rectangle {
          width: parent.width
          height: 100
          color: "transparent"
          border.color: "#555555"
          border.width: 1
          radius: 5

          Image {
            id: imagePreview
            anchors.fill: parent
            anchors.margins: 5
            fillMode: Image.PreserveAspectFit
            source: ""
            visible: source !== ""
          }

          Label {
            anchors.centerIn: parent
            text: "Image preview will appear here"
            color: "#777777"
            visible: imagePreview.source === ""
          }
        }
      }

      // Sound file selector
      Column {
        width: parent.width
        spacing: 8

        Label {
          text: "Sound File (MP3)"
          font.pixelSize: 16
          color: Material.foreground
        }

        Row {
          width: parent.width
          spacing: 10

          TextField {
            id: soundFilePathField
            width: parent.width - soundFileButton.width - 10
            readOnly: true
            placeholderText: "No sound file selected"
            color: Material.foreground

            background: Rectangle {
              color: Material.background
              radius: 5
              border.color: "#555555"
              border.width: 1
            }
          }

          Button {
            id: soundFileButton
            text: "Browse"
            Material.background: Material.primary
            Material.foreground: "#FFFFFF"

            onClicked: soundFileDialog.open()
          }
        }
      }

      // Button color selector
      Column {
        width: parent.width
        spacing: 8

        Label {
          text: "Button Color"
          font.pixelSize: 16
          color: Material.foreground
        }

        Row {
          width: parent.width
          spacing: 20

          // Color preview
          Rectangle {
            id: colorPreview
            width: 50
            height: 50
            color: soundButton1
            radius: 5
            border.color: Material.foreground
            border.width: 1
          }

          // Preset color options
          Grid {
            columns: 3
            columnSpacing: 8
            rowSpacing: 8

            Repeater {
              model: [
                soundButton1, soundButton2, soundButton3,
                soundButton4, soundButton5, soundButton6
              ]

              delegate: Rectangle {
                width: 40
                height: 40
                color: modelData
                radius: 5
                border.color: Material.foreground
                border.width: 1

                MouseArea {
                  anchors.fill: parent
                  onClicked: {
                    colorPreview.color = modelData
                  }
                }
              }
            }
          }

          Button {
            text: "Custom"
            Material.background: Material.primary
            Material.foreground: "#FFFFFF"

            onClicked: colorDialog.open()
          }
        }
      }

      Item {
        width: parent.width
        height: 20
      }

      // Action buttons
      Row {
        width: parent.width
        spacing: 10

        Button {
          text: "Cancel"
          width: parent.width / 2 - 5
          Material.background: "#444444"
          Material.foreground: "#FFFFFF"

          onClicked: {
            addSoundDialog.close()
          }
        }

        Button {
          text: "Add Sound"
          width: parent.width / 2 - 5
          Material.background: Material.accent
          Material.foreground: "#000000"
          font.bold: true

          onClicked: {
            addSoundDialog.close()
            SoundpadController.add(soundNameField.text,imagePathField.text,soundFilePathField.text,colorPreview.color)
            soundNameField.text = ""
            imagePathField.text = ""
            soundFilePathField.text = ""
            imagePreview.source = ""

          }
        }
      }
    }
  }


  // File dialog for image selection
  FileDialog {
    id: imageFileDialog
    title: "Select Image"
    nameFilters: ["Image files (*.png *.jpg *.jpeg *.gif *.svg)"]

    onAccepted: {
      imagePathField.text = selectedFile
      imagePreview.source = selectedFile
    }
  }

  // File dialog for sound selection
  FileDialog {
    id: soundFileDialog
    title: "Select Sound"
    nameFilters: ["Sound files (*.mp3 *.wav *.ogg)"]

    onAccepted: {
      soundFilePathField.text = selectedFile

    }
  }

  // Color dialog for custom color
  ColorDialog {
    id: colorDialog
    title: "Select Custom Color"

    onAccepted: {
      colorPreview.color = colorDialog.selectedColor
    }

  }

}

