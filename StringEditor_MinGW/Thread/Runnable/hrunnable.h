#ifndef HRUNNABLE_H
#define HRUNNABLE_H

#include <QObject>
#include <QRunnable>
#include "Handle/sql.h"

class HRunnable : public QObject, public QRunnable
{
    Q_OBJECT

protected:
    Sql sql;    //数据库类

signals:
    void finished();    //完成信号

    /// 函数功能: 发出当前线程进度信号
    /// now: 当前进度
    /// length: 总进度
    void progressed(const int &now, const int &length);    //进度信号

public:
    HRunnable();
    ~HRunnable();
};

#endif // HRUNNABLE_H
