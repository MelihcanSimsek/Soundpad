import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.company.SoundpadController

Rectangle {
    id: root
    width: 300
    height: 200
    property alias backgroundColor: root.color
    color: "#121212"

    property real minFrequency: 0.5  // Deep voice
    property real maxFrequency: 2.0  // Thin voice
    property real currentFrequency: 1.0  // Normal voice
    property string centerImageSource: "qrc:/assets/icons/onmic.png"

    // Theme colors
    property color primaryColor: "#040299"
    property color accentColor: "#00E676"
    property color foregroundColor: "#FFFFFF"


    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 20

        // Image and Text
        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Image {
                id: micImage
                source: centerImageSource
                width: 40
                height: 40
                fillMode: Image.PreserveAspectFit
                Layout.alignment: Qt.AlignVCenter
            }

            Text {
                text: "Voice Pitch Control"
                color: foregroundColor
                font.pixelSize: 18
                font.bold: true
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignVCenter
            }
        }

        // Current frequency display
        Text {
            id: frequencyText
            text: "Voice Frequency: " + currentFrequency.toFixed(2) + "x"
            color: foregroundColor
            font.pixelSize: 16
            Layout.alignment: Qt.AlignHCenter
        }

        // Main slider control
        Rectangle {
            Layout.fillWidth: true
            height: 60
            color: "transparent"

            // Background track
            Rectangle {
                id: sliderTrack
                anchors.centerIn: parent
                width: parent.width
                height: 10
                radius: 5
                color: "#263238"
            }

            // Shows current value
            Rectangle {
                id: coloredTrack
                anchors.left: sliderTrack.left
                anchors.verticalCenter: sliderTrack.verticalCenter
                width: sliderHandle.x + sliderHandle.width/2
                height: sliderTrack.height
                radius: 5

                gradient: Gradient {
                    orientation: Gradient.Horizontal
                    GradientStop { position: 0.0; color: "#7C4DFF" }
                    GradientStop { position: 0.2; color: "#3D5AFE" }
                    GradientStop { position: 0.4; color: "#2979FF" }
                    GradientStop { position: 0.6; color: "#00B0FF" }
                    GradientStop { position: 0.8; color: "#00E5FF" }
                    GradientStop { position: 1.0; color: "#1DE9B6" }
                }
            }

            // Handle
            Rectangle {
                id: sliderHandle
                width: 40
                height: 40
                radius: 20
                color: accentColor
                border.width: 2
                border.color: "#FFFFFF"

                // Position the handle based on current frequency
                x: ((currentFrequency - minFrequency) / (maxFrequency - minFrequency)) *
                   (parent.width - width)

                anchors.verticalCenter: parent.verticalCenter

                MouseArea {
                    anchors.fill: parent
                    drag {
                        target: parent
                        axis: Drag.XAxis
                        minimumX: 0
                        maximumX: sliderTrack.width - sliderHandle.width
                    }

                    onPositionChanged: {
                        // Update frequency based on handle position
                        var newFrequency = minFrequency +
                                          (sliderHandle.x / (sliderTrack.width - sliderHandle.width)) *
                                          (maxFrequency - minFrequency);
                        currentFrequency = newFrequency;

                    }
                }
            }

            // Labels
            Text {
                text: "Deep"
                color: foregroundColor
                font.pixelSize: 14
                anchors.left: parent.left
                anchors.top: sliderTrack.bottom
                anchors.topMargin: 8
            }

            Text {
                text: "Thin"
                color: foregroundColor
                font.pixelSize: 14
                anchors.right: parent.right
                anchors.top: sliderTrack.bottom
                anchors.topMargin: 8
            }
        }

        // Buttons
        Row {
            spacing: 10
            Layout.alignment: Qt.AlignHCenter

            Rectangle {
                width: 60
                height: 40
                radius: 20
                color: "#7C4DFF"

                Text {
                    anchors.centerIn: parent
                    text: "Deep"
                    color: "white"
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        currentFrequency = 0.7;

                    }
                }
            }

            Rectangle {
                width: 60
                height: 40
                radius: 20
                color: "#00B0FF"

                Text {
                    anchors.centerIn: parent
                    text: "Normal"
                    color: "white"
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        currentFrequency = 1.0;

                    }
                }
            }

            Rectangle {
                width: 60
                height: 40
                radius: 20
                color: "#1DE9B6"

                Text {
                    anchors.centerIn: parent
                    text: "Thin"
                    color: "white"
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        currentFrequency = 1.5;
                    }
                }
            }
        }
    }
}
