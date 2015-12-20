#ifndef VIDEOANALYZER_H
#define VIDEOANALYZER_H

#include <QObject>
#include <QMutex>

#include <QAbstractVideoFilter>
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

class VideoAnalyzer: public QObject
{
    Q_OBJECT
public:
    explicit VideoAnalyzer(QObject *parent = 0);

/**
 * @brief Requests the process to start
 *
 * It is thread safe as it uses #mutex to protect access to #_working variable.
 */
void requestWork();
/**
 * @brief Requests the process to abort
 *
 * It is thread safe as it uses #mutex to protect access to #_abort variable.
 */

void  setScene(cv::Mat scene);

void abort();

private:

cv::CascadeClassifier haar_cascade;
cv::Mat _scene;


/**
 * @brief Process is aborted when @em true
 */
bool _abort;
/**
 * @brief @em true when Worker is doing work
 */
bool _working;
/**
 * @brief Protects access to #_abort
 */
QMutex mutex;

void finish();
bool checkPose(int x, int width, int y, int height);

signals:
/**
 * @brief This signal is emitted when the Worker request to Work
 * @sa requestWork()
 */
void sceneAdded();
void workRequested();

/**
 * @brief This signal is emitted when process is finished (either by counting 60 sec or being aborted)
 */
void finished();
void pose_correct();
void pose_incorrect();

public slots:
/**
 * @brief Does something
 *
 */
void doWork();
};

#endif // VIDEOANALYZER_H
