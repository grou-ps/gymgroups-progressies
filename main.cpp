#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QRect>
#include <QScreen>
#include <QDebug>
#include <videofilter.h>
#include <QQmlEngine>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtQml>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<VideoFilter>("com.calibration.opencv", 1, 0, "VideoFilter");

    QQmlApplicationEngine engine;

    QQmlContext *context = engine.rootContext();

    QRect geometry = QGuiApplication::primaryScreen()->availableGeometry();
    context->setContextProperty( "screenHeight", geometry.height());
    context->setContextProperty( "screenWidth", geometry.width());
    qDebug()<<"Screen Height: "<< geometry.height();
    qDebug()<<"Screen Width: "<< geometry.width();

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
