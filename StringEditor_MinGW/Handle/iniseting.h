#ifndef INISETING_H
#define INISETING_H

#include <QSettings>
#include <QTextCodec>

#define DEBUG            true    // 调试数据

namespace IniSetting
{
    void connectIni(const QString &file = "setting.ini");    //初始化ini文件
    QString dataBase();    //数据库文件名
    QString windowTitle();    //窗口名字
    int threadCount();    //线程数量
    QList<int> tableWidth();    //表格宽度
    int tableHeight();    //表格高度
    int createDataCount();    //创建数据量
    int loadDataCount();    //加载数据量
    int fontSize();    //字体大小
    void setTableWidth(const QList<int> &width);    //设置表格宽度
    void setTableHeight(const int &height);    //设置表格高度
    void setFontSize(const int &size);    //设置字体大小
}

#endif // INISETING_H
