#include "capturethread.h"

/**
 * @brief CaptureThread::CaptureThread
 */
CaptureThread::CaptureThread() :
    isThreadRunning(false)
{

}

/**
 * @brief CaptureThread::~CaptureThread
 */
CaptureThread::~CaptureThread()
{

}

/**
 * @brief CaptureThread::stop
 */
void CaptureThread::stop()
{
    isThreadRunning = false;
}

/**
 * @brief CaptureThread::run
 */
void CaptureThread::run()
{
    isThreadRunning = true;

    while(isThreadRunning)  {
        msleep(30);
        emit captureReady();
    }
}
