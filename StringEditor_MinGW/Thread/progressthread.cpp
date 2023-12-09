#include "progressthread.h"

ProgressThread::ProgressThread(QObject *parent) :
    QThread(parent)
{
    connect(this, &QThread::finished, this, &QThread::deleteLater);

    /// 线程池初始化
    int thread_count = IniSetting::threadCount();
    if (thread_count <= 0) thread_count = QThread::idealThreadCount() * 2;
    thread_pool.setMaxThreadCount(thread_count);

}

float ProgressThread::getSmallProgress() const
{
    float num = 0;
    for (const float &n : runnable_progress.values()) num += n;
    return num;
}

void ProgressThread::connectRunnableProgressed(HRunnable *runnable)
{
    connect(runnable, &HRunnable::progressed, this, [=](const int& a, const int& b){
        if (b > 0) runnable_progress[runnable] = (a / (float)b);
        emit progressed(now_progress + getSmallProgress(), runnable_count);
    });
    connect(runnable, &HRunnable::finished, this, [=](){
        runnable_progress.remove(runnable);
        emit progressed(++now_progress + getSmallProgress(), runnable_count);
    });
}
