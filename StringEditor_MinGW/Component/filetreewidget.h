#ifndef FILETREEWIDGET_H
#define FILETREEWIDGET_H

#include <QObject>
#include <QTreeWidget>
#include <QFileIconProvider>
#include <QDir>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QAction>
#include <QMenu>
#include <QDebug>
#include "Handle/definitionvalue.h"

/* 文件资源管理器控件 */
class FileTreeWidget : public QTreeWidget
{
    Q_OBJECT
private:
    QMap<QString, QTreeWidgetItem*> root_path_items;    //所有顶层路径
    QMap<QString, QTreeWidgetItem*> all_path_items;    //所有非文件夹路径
    QList<QTreeWidgetItem*> focus_items;    //选中items
    QStringList show_path_item;    //显示路径

    QAction *remove_action = new QAction("删除该打开文件", this);    //删除选项
    QMenu *menu = new QMenu(this);    //右键菜单

    /// 函数功能: 创建文件item
    /// path: 该文件路径
    /// name: 修改item显示文字, 默认显示文件名
    /// 返回: 初始化好的QTreeWidgetItem*
    QTreeWidgetItem* createFileItem(const QString &path, QString name = "");

    /// 函数功能: 创建文件夹下item
    /// path: 文件夹路径
    /// 返回: 该文件夹下的所有初始化好的QTreeWidgetItem*组
    QList<QTreeWidgetItem*> createFolderItems(const QString &path);

    /// 函数功能: 获得item所有子item
    /// item: 父类item
    /// 返回: 父类item下的所有子QTreeWidgetItem*组
    QList<QTreeWidgetItem*> getItemChildren(QTreeWidgetItem *item) const;

protected:
    void dragEnterEvent(QDragEnterEvent* event);    //拖入事件函数
    void dropEvent(QDropEvent * event);    //处理拖入事件函数

public:
    explicit FileTreeWidget(QWidget *parent);    //构建函数

    /// 函数功能: 判断路径Item存在与TreeWidget中
    /// path: 需判断的路径
    /// 返回: 是否存在
    bool isPathExist(const QString &path) const;

    /// 函数功能: 添加顶层路径
    /// root_path: 顶层的路径名
    /// 返回: 如果路径已存在则不生成返回false, 不存在则生成成功返回true
    bool addRootPath(const QString &root_path);

    /// 函数功能: 获取顶层item
    /// item: 子类item
    /// 返回: 最顶层父类的QTreeWidgetItem*
    QTreeWidgetItem* getTopItem(QTreeWidgetItem *item) const;

    /// 函数功能: 获取item下所有未隐藏的item的绝对路径
    /// topItem: 父类item, 为空时获取所有顶层item
    /// filter_checked: true只返回选中的item, false返回全部item
    /// 返回: topItem下的符合条件的子item的路径 (包括参数topItem)
    QStringList getShowPaths(QTreeWidgetItem *topItem, const bool &filter_checked);
    QStringList getShowPaths(const bool &filter_checked);    // 获取全部

    /// 函数功能: 只显示指定路径的item
    /// front_paths: 身为父类item的绝对路径字符串组
    /// last_paths: 父类item的绝对路径字符串组
    void showPathItem(const QStringList &front_paths, const QList<QStringList> &last_paths);

    /// 函数功能: 只显示对应路径的item
    /// item: 需要设置的item
    /// shown_paths: 只需要显示item的路径
    /// 返回: 参数item的子item是否全部显示, 只要有一个显示就返回true
    bool setShownItem(QTreeWidgetItem *item, const QStringList &shown_paths);

    /// 函数功能: 删除item
    /// item: 需要删除的item
    void removeItem(QTreeWidgetItem *item);

signals:
    /// 函数功能: 处理完拖入事件后发出的信号
    /// path: 拖入进来的文件路径
    void addDrogItem(const QString &path);

    /// 函数功能: 处理完删除item后发出的信号
    /// item: 被删除的item的路径
    void removeFocusItem(const QString &path);
};

#endif // FILETREEWIDGET_H
