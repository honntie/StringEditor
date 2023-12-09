#ifndef EXTRACTSTRINGTHREAD_H
#define EXTRACTSTRINGTHREAD_H

#include <QFileInfo>
#include <QDir>
#include <QThreadPool>
#include "Runnable/extractstringrunnable.h"
#include "progressthread.h"

class ExtractStringThread : public ProgressThread
{
private:
    QStringList extract_files;    // 打开的路径

    /// 函数功能: 获取路径下的所有文件路径
    /// file_path: 父类文件路径
    /// 返回: 路径下的所有文件路径, 如果该路径已经是文件则返回只有该路径的数组
    QStringList getAllFilePath(const QString &dir_path) const;

public:
    explicit ExtractStringThread(QObject *parent = nullptr);
    ExtractStringThread(const QString &path, QObject *parent = nullptr);
    void run() override;

};

#endif // EXTRACTSTRINGTHREAD_H
