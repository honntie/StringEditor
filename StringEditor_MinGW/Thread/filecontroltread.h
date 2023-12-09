#ifndef FILECONTROLTREAD_H
#define FILECONTROLTREAD_H

#include <QThread>
#include <QThreadPool>
#include <QDir>
#include "Runnable/extractstringrunnable.h"
#include "Runnable/exportfilerunnable.h"
#include "Handle/definitionvalue.h"
#include "Handle/iniseting.h"

/* 文件处理线程, 一切文件操作在此线程操作 */
class FileControlTread : public QThread
{
Q_OBJECT

private:
    Sql sql;    // 数据库
    int mode;    //执行模式
    float finish_count;    //打开文件夹时完成数量
    QString path;    //传值单个路径
    QStringList paths;    //传值多个路径
    QMap<QString, QString> path_map;    //传递一对一路径
    QMap<QRunnable*, float> runnable_progress;    //所有Runnable进度

    float getSmallProgress();    //所有Runnable小进度

    /// 函数功能: 获取文件类型runnable
    /// file_path: 文件路径
    /// 返回: 对应文件的runnable, 用于读取文件夹
    HRunnable* getFileRunnable(const QString &file_path);

    /// 函数功能: 链接线程进度
    /// runnable: 需要连接的线程
    void connectRunnableProgressed(HRunnable *runnable);

    /// 函数功能: 获取路径下的所有文件路径
    /// file_path: 父类文件路径
    /// 返回: 路径下的所有文件路径
    QStringList getAllPath(const QString &dir_path) const;

public:
    explicit FileControlTread(QObject *parent = nullptr);
    void run() override;

    QString getPath() const;    //获取该线程的path

    /// 函数功能: 以打开文本文件的模式运行线程
    /// open_path: 打开的文件路径
    void openFileRun(const QString &open_path);

    /// 函数功能: 以导出文本文件的模式运行线程(未实装)
    /// map: key需要导出的文件, value导出的文件位置
    void exportFileRun(const QMap<QString, QString> &map);

    /// 函数功能: 以导出json文件的模式运行线程(未实装)
    /// file_paths: 需要导出的原文件
    /// export_paths: 导出的文件路径
    void exportJsonRun(const QString &export_folder, const QStringList &file_paths);

signals:
    /// 函数功能: 发出当前线程进度信号
    /// now: 当前进度
    /// length: 总进度
    void progressed(const float &now, const int &length);
};

#endif // FILECONTROLTREAD_H
