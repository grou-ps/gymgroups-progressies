import QtQuick 2.5
import QtMultimedia 5.5
import com.calibration.opencv 1.0

Item {

    property alias mouseArea: mouseArea

    anchors.fill:parent

    property bool photoChangeableValue: false


    MouseArea {
        id: mouseArea
        anchors.fill: parent
    }

    Camera {
        id: camera

        imageProcessing.whiteBalanceMode: CameraImageProcessing.WhiteBalanceFlash
        position: Camera.FrontFace

        exposure {
            exposureCompensation: 1.0
            exposureMode: Camera.ExposurePortrait
        }

        flash.mode: Camera.FlashRedEyeReduction

        imageCapture {
            onImageCaptured: {
                photoPreview.source = preview  // Show the preview in an Image
            }
        }
    }

    VideoOutput {
        source: camera
        anchors.fill: parent
        focus : visible // to receive focus and capture key events when visible
        filters: [videoFilter]
    }

    VideoFilter{
            id: videoFilter
            photoChangeable: photoChangeableValue
        }

    Image {
        source: "passport-template.png"
        visible: true
        anchors.centerIn: parent
    }

    Rectangle {
        id:targetx
        width:100
        height:width
        anchors.centerIn: parent
        color: "transparent"
        border.color: "red"
        border.width:10
        radius: width*0.5
    }

    Image {
        id: photoPreview
    }

    Timer {
            interval: 500;
            triggeredOnStart:true;
            running: true; repeat: true
            onTriggered: checkForTargetColor() //
        }


    function checkForTargetColor() {
        if(videoFilter.photoChangeable)
            targetx.border.color="blue" ;
        else
            targetx.border.color="red" ;
    }
}
