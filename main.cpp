#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QRect>
#include <QScreen>
#include <QDebug>
// #include <opencv2/opencv.hpp>
#include <videofilter.h>
#include <QQmlEngine>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtQml>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<VideoFilter>("com.calibration.opencv", 1, 0, "VideoFilter");
    //qmlRegisterSingletonType<VideoFilter>("com.calibration.opencv", 1, 0, "VideoFilter",videofilter_singletontype_provider);

    QQmlApplicationEngine engine;



    QQmlContext *context = engine.rootContext();

    // QObject::connect(filter, cannyThresholdChanged, context,doSomething, Qt::DirectConnection);

    QRect geometry = QGuiApplication::primaryScreen()->availableGeometry();
    context->setContextProperty( "screenHeight", geometry.height());
    context->setContextProperty( "screenWidth", geometry.width());
    qDebug()<<"Screen Height: "<< geometry.height();
    qDebug()<<"Screen Width: "<< geometry.width();

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    // cv::Mat inputImage = cv::imread("/Users/groups/Documents/defne1.jpg");
    // if(!inputImage.empty()) cv::imshow("Display Image", inputImage);

    return app.exec();
}
