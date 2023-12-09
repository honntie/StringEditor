#ifndef EXPORTFILETHREAD_H
#define EXPORTFILETHREAD_H

#include "progressthread.h"
#include "Runnable/exportfilerunnable.h"

class ExportFileThread : public ProgressThread
{
private:
    QMap<QString, QString> files;    // key: 源文件路径; value: 导出的文件路径

public:
    explicit ExportFileThread(QObject *parent = nullptr);

    /// key: 源文件路径; value: 导出的文件路径
    explicit ExportFileThread(const QMap<QString, QString> &file_map, QObject *parent = nullptr);

    void run() override;
};

#endif // EXPORTFILETHREAD_H
