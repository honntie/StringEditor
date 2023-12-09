#ifndef EXTRACTSTRINGRUNNABLE_H
#define EXTRACTSTRINGRUNNABLE_H

#include <QFile>
#include "hrunnable.h"
#include "Handle/definitionvalue.h"

class ExtractStringRunnable : public HRunnable
{
private:
    QString path;    //文件路径

public:
    /// 构建函数
    /// open_file: 打开的文件路径
    ExtractStringRunnable(const QString &open_file);
    void run() override;
};

#endif // EXTRACTSTRINGRUNNABLE_H
