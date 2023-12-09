#include "extractstringthread.h"

ExtractStringThread::ExtractStringThread(QObject *parent)
    : ProgressThread(parent) {}

ExtractStringThread::ExtractStringThread(const QString &open_path, QObject *parent)
    : ExtractStringThread(parent)
{
    if (QFileInfo(open_path).isFile()) extract_files.append(open_path);
    else extract_files.append(getAllFilePath(open_path));
}

void ExtractStringThread::run()
{
    /// 没有文件
    if (extract_files.isEmpty()) return;
    /// 打开单个文件
    else if (extract_files.length() == 1)
    {
        ExtractStringRunnable *runnable = new ExtractStringRunnable(extract_files.first());
        connect(runnable, &ExtractStringRunnable::progressed, this, &ExtractStringThread::progressed);
        thread_pool.start(runnable);
    }
    /// 打开多个文件
    else
    {
        runnable_count = extract_files.length();
        for (const QString &file_path : extract_files)
        {
            ExtractStringRunnable *runnable = new ExtractStringRunnable(file_path);
            connectRunnableProgressed(runnable);
            thread_pool.start(runnable);
        }
    }
    thread_pool.waitForDone();
}

QStringList ExtractStringThread::getAllFilePath(const QString &dir_path) const
{
    QStringList all_path;
    QDir dir(dir_path);
    QString folder_path;    //文件夹路径
    QString file_path;    //文件路径
    QStringList folder_paths = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList file_paths = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);

    /// 目录下文件夹下文件
    for (QString short_path : folder_paths)
    {
        folder_path = dir_path + "/" + short_path;
        all_path.append(getAllFilePath(folder_path));
    }

    /// 目录下文件
    for (QString short_path : file_paths)
    {
        file_path = dir_path + "/" + short_path;
        all_path.append(file_path);
    }
    return all_path;
}
