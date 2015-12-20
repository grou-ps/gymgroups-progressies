TEMPLATE = app

QT += qml quick
QT += multimedia
QTPLUGIN += qavfcamera

INCLUDEPATH += /usr/local/include/opencv2
INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_calib3d -lopencv_imgproc -lopencv_features2d -lopencv_objdetect -lopencv_contrib

CONFIG += c++11

HEADERS += \
    videofilter.h \
    videoanalyzer.h

SOURCES += main.cpp videofilter.cpp videoanalyzer.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

