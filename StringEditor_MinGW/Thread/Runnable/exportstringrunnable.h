#ifndef EXPORTSTRINGRUNNABLE_H
#define EXPORTSTRINGRUNNABLE_H

#include <QRunnable>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include "Handle/sql.h"

/**************** 字符串导出类 ****************/
class ExportStringRunnable : public QObject, public QRunnable
{
    Q_OBJECT

public:
    ExportStringRunnable(int id, QString user_file);
    void run() override;

    int thread_id;

signals:
    void finished();

private:
    QString file_path;    //文件路径
};

#endif // EXPORTSTRINGRUNNABLE_H
