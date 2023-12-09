#ifndef EXPORTSTRINGTHREAD_H
#define EXPORTSTRINGTHREAD_H

#include <QDir>
#include "progressthread.h"
#include "Handle/sql.h"

class ExportStringThread : public ProgressThread
{

private:
    QString json_path;
    QStringList paths;
    bool is_filiter;

public:
    explicit ExportStringThread(QObject *parent = nullptr);
    void run() override;

    /// export_folder: 导出的文件夹位置
    /// file_paths: 需要导出的文件
    /// filiter: 是否导出过滤的文件
    explicit ExportStringThread(const QString &export_folder, const QStringList &file_paths, const bool &filiter, QObject *parent = nullptr);
};

#endif // EXPORTSTRINGTHREAD_H
