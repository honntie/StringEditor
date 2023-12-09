#ifndef FILEEXPORTFILERUNNABLE_H
#define FILEEXPORTFILERUNNABLE_H


#include <QFile>
#include "hrunnable.h"
#include "Handle/definitionvalue.h"

class FileExportFileRunnable : public HRunnable
{
private:
    QString file_path;    //原文件路径
    QString export_path;    //导出文件位置

public:
    /// 构建函数
    /// file: 原文件路径
    /// export: 导出文件位置
    FileExportFileRunnable(const QString &file, const QString &export);

    void run() override;
};

#endif // FILEEXPORTFILERUNNABLE_H
