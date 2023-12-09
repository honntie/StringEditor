#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextBlock>
#include <QLabel>
#include <QTimer>
#include "Handle/sql.h"
#include "Handle/historyaction.h"
#include "Handle/definitionvalue.h"
#include "Thread/filecontroltread.h"
#include "Thread/exportfilethread.h"
#include "Thread/exportstringthread.h"
#include "Thread/extractstringthread.h"
#include "Thread/importstringthread.h"
#include "Component/hsqltablewidget.h"
#include "replacewindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;    //ui界面
    QTime wait_time;    //消耗时间
    QString last_highlight_string;    //上次高亮字符串

    QLabel *log = new QLabel("欢迎使用？？？", this);    //底部日志
    ReplaceWindow *replace_win = new ReplaceWindow(this);    //替换窗口

    HistoryAction<QMap<QString, QString>> history;    //历史记录
    QMap<QString, QString> now_action;    //当前操作

    /// 函数功能: 连接线程的进度信号
    /// thread: 需要连接的线程
    /// function: 完成时操作的函数
    template<typename FinishFunction>
    void readyThread(ProgressThread *thread, const FinishFunction &finished)
    {
        wait_time = QTime::currentTime();
        this->setEnabled(false);

        //显示进度
        connect(thread, &ProgressThread::progressed, this, [=](float a, int b){
            log->setText(QString("当前进度: %1 / %2").arg(QString::number(a, 'f', 2)).arg(QString::number(b)));
        });

        //线程完成
        connect(thread, &ProgressThread::finished, this, [=](){
            showTreeItems();
            this->setEnabled(true);
            finished();
        });
    }

    /// 函数功能: 添加记录
    /// action: 需要添加的操作
    void appendHistory(const QMap<QString, QString> &action);

    /// 函数功能: 点击导出文件的item
    /// is_checked: true只导出选中的, false导出所有文件
    void exportFileMode(bool is_checked);

    /// 函数功能: 开启打开文件线程
    /// path: 文件路径
    void openFileThread(const QString &path);

    /// 函数功能: 开启读取Json线程
    /// path: 文件路径
    void openJsonThread(const QString &path);

    /// 函数功能: 开启导出文件线程
    /// path_map: key需要导出的文件, value导出的文件位置
    void exportFileThread(const QMap<QString, QString> &path_map);

    /// 函数功能: 开启导出json的线程
    /// export_path: 导出的路径位置
    /// file_paths: 需要导出的文件, 使用默认的空值为导出所有文件
    void exportJsonThread(const QString &export_path, const QStringList &file_paths = QStringList());

    /// 函数功能: 更新HSqlTableWidget的显示设置
    /// widget: 需要更新的HSqlTableWidget
    void updateTableState(HSqlTableWidget *widget);

    /// 函数功能: 替换数据库字符串数据
    /// dict: 需要替换的数据
    /// 返回: 替换的数量
    int replaceSqlString(const QMap<QString, QString> &dict);

    /// 函数功能: 替换当前表格的字符串
    /// source_str: 需要替换的部分
    /// replace_str: 替换成的部分
    /// 返回: 替换的数据
    QMap<QString, QString> replaceCurrentWidgetString(const QString &source_str, const QString &replace_str);

    /// 函数功能: 追加需要替换的字符串
    /// str_map: 追加的字典
    /// source: 需要替换的部分
    /// replace: 替换成的部分
    void appendReplaceString(QMap<QString, QString> &str_dict, const QString &source, const QString &replace) const;

private slots:
    /// 函数功能: 打开文件事件
    /// path: 文件路径
    void openEvent(const QString &path);

    /// 函数功能: 更新选项卡内表格
    /// index: 选项卡位置
    void updateCurrentWidget(const int &index);

    void updateWidget(int index);    //即将放弃

    void showTreeItems();    //显示文件资源管理器的items

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
