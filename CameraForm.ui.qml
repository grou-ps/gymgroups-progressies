import QtQuick 2.5
import QtMultimedia 5.5
import com.calibration.opencv 1.0

Item {

    property alias mouseArea: mouseArea

    anchors.fill:parent

    property double gaussianBlurCoefValue: 1.5 // 1.5
        property int gaussianBlurSizeValue: 7 // 7
        property double cannyThresholdValue: 0.0
        property int cannyKernelSizeValue: 3 //3


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
            gaussianBlurCoef: gaussianBlurCoefValue
            gaussianBlurSize: gaussianBlurSizeValue
            cannyThreshold: cannyThresholdValue
            cannyKernelSize: cannyKernelSizeValue
        }

    Image {
        source: "passport-template.png"
        visible: true
        anchors.centerIn: parent
    }

    Rectangle {
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
}
