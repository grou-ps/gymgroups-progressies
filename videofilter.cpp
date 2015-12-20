// https://github.com/esokullu/Qt_OpenCV

#include <videofilter.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
// #include <opencv2/imgcodecs.hpp>
// #include "opencv2/videoio.hpp"
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
    //int m = *n;
    // qDebug() << "filter runnable address is: " << &n;
    // qDebug() << "filter runnable address is: " << n;
    // qDebug() << "filter runnable address is: " << &n;
    //qDebug() << context.contextProperty("gaussianBlurSizeValue");
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

/*
int VideoFilter::gaussianBlurSize() const
{
    return m_gaussianBlurSize;
}

void VideoFilter::setGaussianBlurSize(int gaussianBlurSize)
{
    m_gaussianBlurSize = gaussianBlurSize %2 == 1 ? gaussianBlurSize : m_gaussianBlurSize;
}

double VideoFilter::gaussianBlurCoef() const
{
    return m_gaussianBlurCoef;
}

void VideoFilter::setGaussianBlurCoef(double gaussianBlurCoef)
{
    m_gaussianBlurCoef = gaussianBlurCoef;
}

double VideoFilter::cannyThreshold() const
{
    return m_cannyThreshold;
}

void VideoFilter::setCannyThreshold(double cannyThreshold)
{
    m_cannyThreshold = cannyThreshold;
}

int VideoFilter::cannyKernelSize() const
{
    return m_cannyKernelSize;
}

void VideoFilter::setCannyKernelSize(int cannyKernelSize)
{

    m_cannyKernelSize = cannyKernelSize%2 == 1 ? cannyKernelSize : m_cannyKernelSize;
}
*/



FilterRunnable::FilterRunnable(VideoFilter *filter) :
    m_filter(filter)
{



    thread = new QThread();
    worker = new VideoAnalyzer();

    worker->moveToThread(thread);

    qDebug() << "FilterRunnable called";

    //connect(worker, SIGNAL(valueChanged(QString)), ui->label, SLOT(setText(QString)));
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
    //qDebug()<<"Deleting thread and worker in Thread "<<this->QObject::thread()->currentThreadId();
    delete thread;
    delete worker;
}

QVideoFrame FilterRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, QVideoFilterRunnable::RunFlags flags)
{

    //int gaussianBlurSize = m_filter->gaussianBlurSize();
    //double gaussianBlurCoef = m_filter->gaussianBlurCoef();
    //int cannyKernelSize = m_filter->cannyKernelSize();
    //double cannyThreshold = m_filter->cannyThreshold();
    //float sigma = 0.33;


    //vector<Mat> images;
    //vector<int> labels;
    //read_csv(,images,labels);
    //Ptr<FaceRecognizer> model = createFisherFaceRecognizer();
    //model->train(images, labels);




    if (!input->isValid())
        return *input;

    input->map(QAbstractVideoBuffer::ReadWrite);

    //this->deleteColorComponentFromYUV(input);

    cv::Mat mat(input->height(),input->width(), CV_8U, input->bits());

    worker->setScene(mat);


    // this->haar_cascade.detectMultiScale(mat, faces);
    //qDebug() << "Face is: " << faces[0].x << faces[0].width << faces[0].y << faces[0].height;

    /*

    cv::Scalar avgPixelIntensity = cv::mean( mat );
    int avgPixelIntensityInt = int(avgPixelIntensity[0]);
    int newLowThreshold = int(std::max(0.0, (1.0-sigma) * avgPixelIntensityInt));
    int newHighThreshold = int(std::min(255.0, (1.0+sigma) * avgPixelIntensityInt));


    cv::GaussianBlur(mat, mat, Size(gaussianBlurSize,gaussianBlurSize), gaussianBlurCoef, gaussianBlurCoef);
    //cv::Canny(mat, mat, cannyThreshold, 3 * cannyThreshold, cannyKernelSize);
    cv::Canny(mat, mat, newLowThreshold, newHighThreshold, cannyKernelSize);


    int white_spaces = 0;
    int center_x = mat.cols/2;
    int center_y = mat.rows/2;
    int current_x = 0;
    int current_y = 0;
    for(int i=1;i<=360;i++) {
        for(int j=50;j<=60;j++) {
            current_x = round(center_x+cos(i)*j);
            current_y = round(center_y+sin(i)*j);
            if(
                    *mat.ptr(current_x,current_y,false)==255
                    ||
                    *mat.ptr(current_x-1,current_y-1,false)==255
                    ||
                    *mat.ptr(current_x+1,current_y+1,false)==255
                    ||
                    *mat.ptr(current_x,current_y+1,false)==255
                    ||
                    *mat.ptr(current_x,current_y-1,false)==255
                    ||
                    *mat.ptr(current_x-1,current_y+1,false)==255
                    ||
                    *mat.ptr(current_x-1,current_y,false)==255
                    ||
                    *mat.ptr(current_x+1,current_y-1,false)==255
                    ||
                    *mat.ptr(current_x+1,current_y,false)==255
                    ) {
                white_spaces++;
            }
        }

    }

    qDebug() << "Number of whites" << white_spaces;

    //qDebug() << "Number of rows" << mat.rows;


    //http://qtandopencv.blogspot.com/2013/09/derivative-and-edge-in-image-processing.html
    // http://study.marearts.com/2013/01/cvfilter2d-example-source-code-various.html
*/

    input->unmap();
    return *input;

}

/*
void FilterRunnable::deleteColorComponentFromYUV(QVideoFrame *input)
{
    // Assign 0 to Us and Vs
    int firstU = input->width()*input->height();
    int lastV = input->width()*input->height()+2*input->width()*input->height()/4;
    uchar* inputBits = input->bits();

    for (int i=firstU; i<lastV; i++)
        inputBits[i] = 127;
}

*/
