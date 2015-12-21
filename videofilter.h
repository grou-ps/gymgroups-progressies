#ifndef VIDEOFILTER
#define VIDEOFILTER

#include <QAbstractVideoFilter>
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
#include <videoanalyzer.h>

#include <QQmlEngine>
#include <QJSEngine>



class VideoFilter : public QAbstractVideoFilter
{
    Q_OBJECT

    Q_PROPERTY(bool photoChangeable READ photoChangeable WRITE setPhotoChangeable NOTIFY photoChangeableChanged)
public:
    QVideoFilterRunnable *createFilterRunnable() Q_DECL_OVERRIDE;

    bool photoChangeable() const;
    void setPhotoChangeable(bool mode);

private:
    bool m_photoChangeable;

signals:
    void photoChangeableChanged();

public slots:
    void allowPhoto();
    void disallowPhoto();

};




class FilterRunnable : public QVideoFilterRunnable
{
public:
    FilterRunnable(VideoFilter *filter);
    ~FilterRunnable();

    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags) Q_DECL_OVERRIDE;

private:
    VideoFilter *m_filter;


    /**
     * @brief Thread object which will let us manipulate the running thread
     */
    QThread *thread;
    /**
     * @brief Object which contains methods that should be runned in another thread
     */
    VideoAnalyzer *worker;

    // void deleteColorComponentFromYUV(QVideoFrame *input);

};

#endif // VIDEOFILTER
