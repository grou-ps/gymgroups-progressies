#ifndef VIDEOANALYZER_H
#define VIDEOANALYZER_H

#include <QObject>
#include <QMutex>

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
void abort();

private:
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

signals:
/**
 * @brief This signal is emitted when the Worker request to Work
 * @sa requestWork()
 */
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