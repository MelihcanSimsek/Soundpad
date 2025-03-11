import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Dialogs
import com.company.SoundpadController



// RED - #8E1616
// ORANGE - #F14A00
Window {
  width: 800
  height: 800
  minimumHeight: 800
  minimumWidth: 800
  visible: true
  title: "Soundpad"


  Material.theme: Material.Dark
  Material.primary: "#040299"
  Material.accent: "#00E676"
  Material.foreground: "#FFFFFF"
  Material.background: "#121212"



  Rectangle{
    id:topSection
    anchors{
      left: parent.left
      right: parent.right
      top:parent.top
    }

    height: 90
    color:Material.background

    Row{
      anchors{
        left:parent.left
        verticalCenter: parent.verticalCenter
        leftMargin: 30
      }

      spacing: 20

      ImageButton{
        id:folderIcon
        baseImage:"qrc:/assets/icons/folder.png"
        width: 64
        height: 64
        onClicked:{
          addSoundDialog.open()
        }
      }

      ImageButton{
        id:deleteIcon
        baseImage:"qrc:/assets/icons/delete.png"
        width: 64
        height: 64
        onClicked: {
          SoundpadController.changeDeletingMode()
        }
      }

      ImageButton{
        id:volumeIcon
        baseImage:"qrc:/assets/icons/volume.png"
        width: 64
        height: 64
        onClicked: {
          SoundpadController.openVolumeDialog()
          audioVolumeSettingsDialog.open()
        }
      }

    }

    Row{
      anchors{
        right: parent.right
        verticalCenter: parent.verticalCenter
        rightMargin: 30
      }

      spacing: 20


      ImageButton{
        id:exitButton
        baseImage:"qrc:/assets/icons/exit.png"
        width: 64
        height: 64
        onClicked: {
          Qt.exit(1)
        }
      }

    }



  }

  Rectangle{
    id:middleSection
    anchors{
      left:parent.left
      right: parent.right
      top:topSection.bottom
      bottom: parent.bottom
    }

    color:Material.accent

    SwipeView {
      id: view

      currentIndex: 0
      anchors.fill: parent

      Soundpad{
        id:soundpadPage
        backgroundColor:Material.background

      }

      Wheel{
        id: microphonePage
        clip: true
        backgroundColor:Material.background
      }
    }

    PageIndicator {
      id: indicator

      count: view.count
      currentIndex: view.currentIndex

      anchors.bottom: view.bottom
      anchors.horizontalCenter: parent.horizontalCenter
    }

  }

  VolumeDialog{
    id:audioVolumeSettingsDialog
  }

  AddDialog{
    id:addSoundDialog
  }
}

