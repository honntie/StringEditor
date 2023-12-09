#ifndef EXPORTFILERUNNABLE_H
#define EXPORTFILERUNNABLE_H

#include <QFile>
#include <QDir>
#include "hrunnable.h"
#include "Handle/definitionvalue.h"


/**************** 文件导出类 ****************/
class ExportFileRunnable : public HRunnable
{
    Q_OBJECT

public:
    ExportFileRunnable(QString source_file, QString export_file);
    void run() override;

private:
    QString file_path;    //文件路径
    QString export_path;    //导出路径
};

#endif // EXPORTFILERUNNABLE_H
