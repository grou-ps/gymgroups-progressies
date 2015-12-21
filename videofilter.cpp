#include <videofilter.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <algorithm>
#include <cmath>
#include <QDebug>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <QThread>

using namespace cv;



QVideoFilterRunnable *VideoFilter::createFilterRunnable()
{
    FilterRunnable *n = new FilterRunnable(this);
    return n;
}




void VideoFilter::allowPhoto() {
    setPhotoChangeable(true);
}

void VideoFilter::disallowPhoto() {
    setPhotoChangeable(false);
}

bool VideoFilter::photoChangeable() const {
    return m_photoChangeable;
}

void VideoFilter::setPhotoChangeable(bool mode) {
    m_photoChangeable = mode;
}


FilterRunnable::FilterRunnable(VideoFilter *filter) :
    m_filter(filter)
{
    thread = new QThread();
    worker = new VideoAnalyzer();
    worker->moveToThread(thread);
    qDebug() << "FilterRunnable called";

    QObject::connect(worker, SIGNAL(sceneAdded()), thread, SLOT(start()));
    QObject::connect(thread, SIGNAL(started()), worker, SLOT(doWork()));
    QObject::connect(worker, SIGNAL(finished()), thread, SLOT(quit()), Qt::DirectConnection);
    QObject::connect(worker, SIGNAL(pose_correct()), filter, SLOT(allowPhoto()), Qt::DirectConnection);
    QObject::connect(worker, SIGNAL(pose_incorrect()), filter, SLOT(disallowPhoto()), Qt::DirectConnection);
}

FilterRunnable::~FilterRunnable()
{
    worker->abort();
    thread->wait();

    delete thread;
    delete worker;
}

QVideoFrame FilterRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, QVideoFilterRunnable::RunFlags flags)
{

    if (!input->isValid())
        return *input;

    input->map(QAbstractVideoBuffer::ReadWrite);
    cv::Mat mat(input->height(),input->width(), CV_8U, input->bits());
    worker->setScene(mat);

    input->unmap();
    return *input;

}
