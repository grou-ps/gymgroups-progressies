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


void VideoAnalyzer::doWork()
{
    qDebug()<<"Starting worker process in Thread "<<thread()->currentThreadId();

    for (int i = 0; i < 60; i ++) {

        // Checks if the process should be aborted
        mutex.lock();
        bool abort = _abort;
        mutex.unlock();

        if (abort) {
            qDebug()<<"Aborting worker process in Thread "<<thread()->currentThreadId();
            break;
        }

        // This will stupidly wait 1 sec doing nothing...
        //QEventLoop loop;
        //QTimer::singleShot(1000, &loop, SLOT(quit()));
        //loop.exec();


        if(false) {
            emit pose_correct();
        }
        else {
            emit pose_incorrect();
        }
    }

    // Set _working to false, meaning the process can't be aborted anymore.
    mutex.lock();
    _working = false;
    mutex.unlock();

    qDebug()<<"Worker process finished in Thread "<<thread()->currentThreadId();

    //Once 60 sec passed, the finished signal is sent
    emit finished();
}

