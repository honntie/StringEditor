#ifndef HSQLTABLEWIDGET_H
#define HSQLTABLEWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QScrollBar>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QDateTime>
#include <QKeyEvent>
#include <QWheelEvent>
#include "Handle/sql.h"
#include "Handle/definitionvalue.h"

/* 数据库可编辑表格控件 */
class HSqlTableWidget : public QWidget
{
    Q_OBJECT

private:
    QLineEdit *search_edit = new QLineEdit(this);    //搜索框
    QVBoxLayout* layout = new QVBoxLayout(this);    //布局
    QTableWidget *view = new QTableWidget(this);    //数据库表格
    bool is_ctrl = false;    //是否按着ctrl

    TableData *load_data = nullptr;    //加载数据
    long data_size;    //数据数组长度
    short sort_num[4] = {0, 1, 2, 3};    //排序先后
    bool is_desc = false;    //排序规则

    QString path;    //显示路径

    QString getDataQuery() const;    //获取数据语句
    QString getSizeQuery() const;    //获取数据库长度语句
    void createLoadData();    //创建新数据
    int compareData(const TableData &data1, const TableData &data2);    //比较方法
    void quickSort(TableData *array, int length, int begin = 0);    //快速排序
    void sortData();    //排序数组函数

    ////////////////////////////////////
    /// 函数功能: 初始化某行数据
    /// row: 行数位置
    inline void iniRowItems(const int &row);

    ////////////////////////////////////
    /// 函数功能: 判断是否需要隐藏数
    /// data: 用于判断的数据
    /// 返回: 是否隐藏
    inline bool isHiddenData(const TableData &data) const;

    ////////////////////////////////////
    /// 函数功能: 追加一行数据 (不使用)
    /// data: 添加的数据
    inline void appendRowData(const TableData &data);

    ////////////////////////////////////
    /// 函数功能: 更新某行数据
    /// row: 需要设置的行数位置
    inline void updateRowData(const int &row);

    ////////////////////////////////////
    /// 函数功能: 将数据库当前位置的数据转成TableData
    /// query: 执行数据库的语句 (没有做任何检验)
    TableData getSqlData(const QSqlQuery &query) const;

    ////////////////////////////////////
    /// 函数功能: 表格发生修改时进行数据库更新
    /// item: 被修改的单元格
    void sqlUpdate(const QTableWidgetItem *item);

    ////////////////////////////////////
    /// 函数功能: 字符串比较大小
    /// str1: 左边字符串
    /// str2: 右边字符串
    /// 返回: 小于0:str1大, 等于0:一样大, 大于0:str2大
    inline short compareString(const QString &str1, const QString &str2);

public:
    bool show_hidden_data = false;    //显示过滤数据语句
    bool show_not_changed_data = false;    //显示修改字符串语句
    bool filter_data = false;    //查找字符串语句
    bool show_same_data = false;    //显示重复字符串语句

    void createTable();    //创建表格
    void updateCurrentItems();    //更新当前在显示的表格
    void updateTable();    //更新表格
    void hideDataRow();    //隐藏不符合条件数据

    QString getPath() const;    //返回当前路径

    ~HSqlTableWidget();    //析构函数

    /// 构建函数
    /// file_path: 显示数据库中的路径
    /// parent: 控件父类
    explicit HSqlTableWidget(const QString &file_path, QWidget *parent = nullptr);

    /// 函数功能: 设置显示原字符串
    /// is_show_source_data: true显示, false不显示
    void setShowSourceData(const bool &is_show_source_data);

    /* 遗弃代码
    /// 函数功能: 设置只显示隐藏数据
    /// is_show_hide_data: true只显示隐藏数据, false不显示隐藏数据
    void setShowHideData(const bool &is_show_hide_data);

    /// 函数功能: 设置只显示重复
    /// is_show_same: true只显示重复数据, false不显示重复数据
    void setShowSame(const bool &is_show_same);

    /// 函数功能: 设置显示未修改
    /// is_show_not_translate: true只显示修改数据, false显示全部数据
    void setShowNotTran(const bool &is_show_not_translate);
    */

    /// 函数功能: 获取表格中包含该字符串的数据
    /// contains: 包含的字符串数据
    /// 返回: 符合条件的QTableWidgetItem*组
    QList<QTableWidgetItem*> getStringDatas(const QString &contains = "") const;

    /// 函数功能: 替换部分数据 (模糊)
    /// source: 原字符串
    /// replace: 修改后字符串
    /// 返回: 需要修改的字典key: 原字符串    value: 修改字符串
    QMap<QString, QString> replaceStringData(const QString &source, const QString &replace);

    /// 函数功能: 替换部分数据 (全词匹配)
    /// source: 原字符串
    /// replace: 修改后字符串
    void changeStringData(const QString &source, const QString &replace);


signals:
    /// 函数功能: 表格被点击时发出信号，用于显示预览
    /// path: 被点击的文件路径
    /// index: 字符串位置
    /// string: 原字符串
    void tableClicked(const QString &path, const int &index, const QString &string);

    /// 函数功能: 表格被修改时发出信号，用于撤回与恢复
    /// source: 原字符串
    /// changed: 修改后字符串
    void tableChange(const QString &source, const QString &changed);

protected:
    void keyPressEvent(QKeyEvent *et);    //键盘按下事件
    void keyReleaseEvent(QKeyEvent *et);    //键盘松开事件
};

#endif // HSQLTABLEWIDGET_H
