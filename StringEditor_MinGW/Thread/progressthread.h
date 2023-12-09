#ifndef PROGRESSTHREAD_H
#define PROGRESSTHREAD_H

#include <QThread>
#include <QThreadPool>
#include "Runnable/hrunnable.h"

class ProgressThread : public QThread
{
    Q_OBJECT

private:
    float getSmallProgress() const;    //所有Runnable小进度

protected:
    QThreadPool thread_pool;    // 线程池
    QMap<HRunnable*, float> runnable_progress;    //所有Runnable进度
    float now_progress = 0;    // 线程完成数
    int runnable_count;    // 线程总数

    /// 函数功能: 链接线程进度
    /// runnable: 需要连接的线程
    void connectRunnableProgressed(HRunnable *runnable);

public:
    explicit ProgressThread(QObject *parent = nullptr);

signals:
    /// 函数功能: 发出当前线程进度信号
    /// now: 当前进度
    /// length: 总进度
    void progressed(const float &now, const int &length);
};

#endif // PROGRESSTHREAD_H
