import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Imagine 2.12
import QtQuick.Window 2.0
Rectangle {
    id: mainview
    color: "black"
    Layout.fillHeight: true
    state: "stopped"
    property bool isHoverEnabled: true

    readonly property color colorGlow: "#1d6d64"
    readonly property color colorWarning: "#d5232f"
    readonly property color colorMain: "#6affcd"
    readonly property color colorBright: "#ffffff"
    readonly property color colorLightGrey: "#888"
    readonly property color colorDarkGrey: "#333"

    readonly property int fontSizeExtraSmall: Qt.application.font.pixelSize * 0.8
    readonly property int fontSizeMedium: Qt.application.font.pixelSize * 1.5
    readonly property int fontSizeLarge: Qt.application.font.pixelSize * 2
    readonly property int fontSizeExtraLarge: Qt.application.font.pixelSize * 5

    property alias roverXYText: roverXY.text
    property alias bachelorXYText: bachelorXY.text
    property alias weddingXYText: weddingXY.text
	property alias roverStepsText: roverSteps.text
	property alias bachelorStepsText: bachelorSteps.text

    states: [
		State {
			name: "playing"
			PropertyChanges {
				target: playButtonImage
				source: {
					if (playButtonMouseArea.containsMouse)
						"qrc:/images/pausehoverpressed.png"
					else
						"qrc:/images/pausenormal.png"
				}
			}
			PropertyChanges {
				target: stopButtonImage
				source: "qrc:/images/stopnormal.png"
			}
		},
		State {
			name: "paused"
			PropertyChanges {
				target: playButtonImage
				source: {
					if (playButtonMouseArea.containsMouse)
						"qrc:/images/playhoverpressed.png"
					else
						"qrc:/images/playnormal.png"
				}
			}
			PropertyChanges {
				target: stopButtonImage
				source: "qrc:/images/stopnormal.png"
			}
		},
		State {
			name: "stopped"
			PropertyChanges {
				target: playButtonImage
				source: "qrc:/images/playnormal.png"
			}
			PropertyChanges {
				target: stopButtonImage
				source: "qrc:/images/stopdisabled.png"
			}
		}
	]

	ColumnLayout {
		spacing: 0
		anchors.fill: parent
		scale: 1
		Button {
			id: btnCalc
			Layout.alignment: Qt.AlignCenter
			text: "Calculate"
			Layout.margins: 9
			Layout.fillWidth: true

			contentItem: Text {
				text: btnCalc.text
				font.bold: true
				styleColor: "#e70c0c"
				font.pointSize: 13
				opacity: enabled ? 1.0 : 0.3
				color: "#0a0a0a"
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
				elide: Text.ElideRight
			}

			background: Rectangle {
				implicitWidth: 100
				implicitHeight: 40
				opacity: enabled ? 1 : 0.3
				color: "#7de47d"
				border.color: btnCalc.down ? "#FFFFFF" : "#00FF00"
				border.width: 1
				radius: 2
			}
			onClicked: {
				btnCalc.enabled = false
				controlsOverlay.visible = false
				playButton.enabled = true
				ffButton.enabled = true
				stopButton.enabled = true
				speedSlider.enabled = true
				pathcalc.calculate()
			}
		}

		GlowingLabel {
			text: qsTr("Animation Speed")
			color: "white"
			font.pointSize: 11
			Layout.rowSpan: 2
		}

		Slider {
			id: speedSlider
			enabled: false
			Layout.rowSpan: 3
			Layout.margins: 5
			Layout.alignment: Qt.AlignLeft | Qt.AlignTop
			Layout.topMargin: 0
			from: 0
			to: 500
			value: 500
			Layout.fillWidth: true
			onValueChanged: animator.changeSpeed(value)
        }

        Rectangle {
            id: rectangle
            Layout.rowSpan: 4
            Layout.topMargin: 0
            layer.enabled: true
            transformOrigin: Item.Center
            Layout.fillWidth: true
            Layout.preferredHeight: 60
            Layout.alignment: Qt.AlignCenter | Qt.AlignTop
            color: "transparent"

            Rectangle {
                id: controlsOverlay
                anchors.fill: playbackControls
                color: "#80efefea"
            }

            RowLayout {
            id: playbackControls
            anchors.fill: parent
            property int buttonHorizontalMargin: 10
            transformOrigin: Item.Center

			Rectangle {
				id: playButton
				enabled: false
				height: 54
				width: 54
                color: "transparent"
                Layout.alignment: Qt.AlignCenter

				Image {
					id: playButtonImage
					source: "qrc:/images/pausenormal.png"
				}

				MouseArea {
					id: playButtonMouseArea
					anchors.fill: parent
					hoverEnabled: isHoverEnabled
					onClicked: {
						if (mainview.state == 'paused' || mainview.state == 'stopped')
							mainview.state = 'playing'
						else
							mainview.state = 'paused'
						animator.togglePlayPause()
					}
					onEntered: {
						if (mainview.state == 'playing')
							playButtonImage.source = "qrc:/images/pausehoverpressed.png"
						else
							playButtonImage.source = "qrc:/images/playhoverpressed.png"
					}
					onExited: {
						if (mainview.state == 'playing')
							playButtonImage.source = "qrc:/images/pausenormal.png"
						else
							playButtonImage.source = "qrc:/images/playnormal.png"
					}
				}
			}
			
			Rectangle {
				id: ffButton
				enabled: false
				height: 54
				width: 54
                color: "transparent"
                Layout.alignment: Qt.AlignCenter


				Image {
					id: ffButtonImage
					source: "qrc:/images/ffnormal.png"
				}

				MouseArea {
					id: ffButtonMouseArea
					anchors.fill: parent
					hoverEnabled: isHoverEnabled
					onClicked: {
						animator.fastForward()
					}
					onEntered: ffButtonImage.source = "qrc:/images/ffhoverpressed.png"
					
					onExited: ffButtonImage.source = "qrc:/images/ffnormal.png"
				}
			}

			Rectangle {
				id: stopButton
				enabled: false
				height: 54
				width: 54
				color: "transparent"
                Layout.alignment: Qt.AlignCenter

				Image {
					id: stopButtonImage
					source: "qrc:/images/stopnormal.png"
				}

				MouseArea {
					anchors.fill: parent
					hoverEnabled: isHoverEnabled
					onClicked: {
						mainview.state = 'stopped'
						animator.stop()
					}
					onEntered: {
						if (mainview.state != 'stopped')
							stopButtonImage.source = "qrc:/images/stophoverpressed.png"
					}
					onExited: {
						if (mainview.state != 'stopped')
							stopButtonImage.source = "qrc:/images/stopnormal.png"
					}
				}
			}
		}


        }
		ColumnLayout {
			Layout.fillWidth: true
			Layout.alignment: Qt.AlignCenter | Qt.AlignTop
			property int buttonHorizontalMargin: 10
			Layout.rowSpan: 5
			Layout.topMargin: 0
			Layout.leftMargin: 0
			transformOrigin: Item.Center

			RowLayout {
				Layout.fillWidth: true
				GlowingLabel {
					text: qsTr("Rover x,y:")
					color: "white"
					font.pointSize: 10
					horizontalAlignment: Text.AlignLeft
					Layout.fillWidth: true
				}
				Label {
					id: roverXY
					color: "#d5e8da"
					font.pointSize: 10
					horizontalAlignment: Text.AlignRight
					Layout.fillWidth: true
				}
			}        

			RowLayout {
				Layout.fillWidth: true
				GlowingLabel {
					text: qsTr("Bachelor x,y:")
					color: "white"
					font.pointSize: 10
					horizontalAlignment: Text.AlignLeft
					Layout.fillWidth: true
				}
				Label {
					id: bachelorXY
					color: "#d5e8da"
					font.pointSize: 10
					horizontalAlignment: Text.AlignRight
					Layout.fillWidth: true
				}
			}

			RowLayout {
				Layout.fillWidth: true
				GlowingLabel {
					text: qsTr("Wedding x,y:")
					color: "white"
					font.pointSize: 10
					horizontalAlignment: Text.AlignLeft
					Layout.fillWidth: true
				}
				Label {
					id: weddingXY
					color: "#d5e8da"
					font.pointSize: 10
					horizontalAlignment: Text.AlignRight
					Layout.fillWidth: true
				}
			}
			
			RowLayout {
				Layout.fillWidth: true
				GlowingLabel {
					text: qsTr("Rover steps:")
					color: "white"
					font.pointSize: 10
					horizontalAlignment: Text.AlignLeft
					Layout.fillWidth: true
				}
				Label {
					id: roverSteps
					color: "#d5e8da"
					font.pointSize: 10
					horizontalAlignment: Text.AlignRight
					Layout.fillWidth: true
				}
			}
			
			RowLayout {
				Layout.fillWidth: true
				GlowingLabel {
					text: qsTr("Bachelor steps:")
					color: "white"
					font.pointSize: 10
					horizontalAlignment: Text.AlignLeft
					Layout.fillWidth: true
				}
				Label {
					id: bachelorSteps
					color: "#d5e8da"
					font.pointSize: 10
					horizontalAlignment: Text.AlignRight
					Layout.fillWidth: true
				}
			}
		}

	}
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
