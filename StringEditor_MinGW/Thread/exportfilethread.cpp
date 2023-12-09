#include "exportfilethread.h"

ExportFileThread::ExportFileThread(QObject *parent)
    : ProgressThread{parent} {}

ExportFileThread::ExportFileThread(const QMap<QString, QString> &file_map, QObject *parent)
    : ProgressThread(parent), files(file_map) {}

void ExportFileThread::run()
{
    /// 没有文件
    if (files.isEmpty()) return;
    /// 导出单个文件
    else if (files.size() == 1)
    {
        ExportFileRunnable *runnable = new ExportFileRunnable(files.firstKey(), files[files.firstKey()]);
        emit progressed(0, 1);
        thread_pool.start(runnable);
    }

    /// 导出多个文件
    else
    {
        runnable_count = files.size();
        for (const QString &path : files.keys())
        {
            ExportFileRunnable *runnable = new ExportFileRunnable(path, files[path]);
            emit progressed(0, files.size());
            connectRunnableProgressed(runnable);
            thread_pool.start(runnable);
        }
    }
    thread_pool.waitForDone();

}
