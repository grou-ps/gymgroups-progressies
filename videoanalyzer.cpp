#include <videoanalyzer.h>

#include <QTimer>
#include <QEventLoop>
#include <QThread>
#include <QDebug>
#include <QObject>
#include <QMutex>

VideoAnalyzer::VideoAnalyzer(QObject *parent):
    QObject(parent)
{
    std::string fn_haar = "/usr/local/share/OpenCV/haarcascades/haarcascade_frontalface_default.xml";
    this->haar_cascade.load(fn_haar);
    _working =false;
    _abort = false;
}
void VideoAnalyzer::requestWork()
{
    mutex.lock();
    _working = true;
    _abort = false;
    qDebug()<<"Request worker start in Thread "<<thread()->currentThreadId();
    mutex.unlock();

    emit workRequested();
}

void VideoAnalyzer::abort()
{
    mutex.lock();
    if (_working) {
        _abort = true;
        qDebug()<<"Request worker aborting in Thread "<<thread()->currentThreadId();
    }
    mutex.unlock();
}

void VideoAnalyzer::setScene(cv::Mat scene) {
    if(!_working) {
        _scene = scene;
        emit sceneAdded();
    }
}

void VideoAnalyzer::finish() {
    // Set _working to false, meaning the process can't be aborted anymore.
    mutex.lock();
    _working = false;
    mutex.unlock();

    qDebug()<<"Worker process finished in Thread "<<thread()->currentThreadId();

    //Once 60 sec passed, the finished signal is sent
    emit finished();
}

void VideoAnalyzer::doWork()
{
    qDebug()<<"Starting worker process in Thread "<<thread()->currentThreadId();


        // Checks if the process should be aborted
        mutex.lock();
        bool abort = _abort;
        mutex.unlock();

        if (abort) {
            qDebug()<<"Aborting worker process in Thread "<<thread()->currentThreadId();
            //break;
            finish();
        }

        // This will stupidly wait 1 sec doing nothing...
        //QEventLoop loop;
        //QTimer::singleShot(1000, &loop, SLOT(quit()));
        //loop.exec();

        cv::vector< cv::Rect_<int> > faces;
        this->haar_cascade.detectMultiScale(_scene, faces);
         qDebug() << "Face is: " << faces[0].x << faces[0].width << faces[0].y << faces[0].height;

        if(checkPose(faces[0].x, faces[0].width, faces[0].y, faces[0].height)) {
            qDebug() << "Good!";
            emit pose_correct();
        }
        else {
            qDebug() <<  "Bad!";
            emit pose_incorrect();
        }

        finish();

}

bool VideoAnalyzer::checkPose(int x, int width, int y, int height) {
    return ( x>450 && x<500
            &&
            width>350&&width<500
            &&
            y>130 && y<190
            &&
            height>350 && height < 500
            );
}
