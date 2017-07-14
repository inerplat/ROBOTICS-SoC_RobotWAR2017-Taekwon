#ifndef CAPTURETHREAD_H
#define CAPTURETHREAD_H

#include <QThread>

class CaptureThread : public QThread
{
    Q_OBJECT
public:
    explicit CaptureThread();
    ~CaptureThread();
    void run();
    void stop();

private:
    bool isThreadRunning;

signals:
    void captureReady();

};

#endif // CAPTURETHREAD_H
