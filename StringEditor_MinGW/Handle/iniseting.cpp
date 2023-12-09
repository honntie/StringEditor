#include "iniseting.h"

namespace IniSetting
{
    QSettings *ini;    //ini文件

    void connectIni(const QString &file)
    {
        ini = new QSettings(file, QSettings::IniFormat);
        ini->setIniCodec(QTextCodec::codecForName("utf-8"));

        ini->beginGroup("system");
        if (!ini->contains("db_name")) ini->setValue("db_name", "info.db");
        if (!ini->contains("thread_count")) ini->setValue("thread_count", "");
        ini->endGroup();

        ini->beginGroup("mainwindow");
        if (!ini->contains("window_title")) ini->setValue("window_title", "未命名");
        if (!ini->contains("table_width_1")) ini->setValue("table_width_1", 150);
        if (!ini->contains("table_width_2")) ini->setValue("table_width_2", 35);
        if (!ini->contains("table_width_3")) ini->setValue("table_width_3", 325);
        if (!ini->contains("table_width_4")) ini->setValue("table_width_4", 325);
        if (!ini->contains("table_width_5")) ini->setValue("table_width_5", 450);
        if (!ini->contains("table_height")) ini->setValue("table_height", 30);
        if (!ini->contains("font_size")) ini->setValue("font_size", 12);
        ini->endGroup();
    }

    QString dataBase()
    {
        QString result;
        ini->beginGroup("system");
        result = ini->value("db_name", "info.db").toString();
        ini->endGroup();
        return result;
    }

    int threadCount()
    {
        int result;
        ini->beginGroup("system");
        result = ini->value("thread_count").toInt();
        ini->endGroup();
        return result;
    }

    QString windowTitle()
    {
        QString result;
        ini->beginGroup("mainwindow");
        result = ini->value("window_title", "未命名").toString();
        ini->endGroup();
        return result;
    }

    QList<int> tableWidth()
    {
        QList<int> result;
        ini->beginGroup("mainwindow");
        result.append(ini->value("table_width_1", 150).toInt());
        result.append(ini->value("table_width_2", 35).toInt());
        result.append(ini->value("table_width_3", 325).toInt());
        result.append(ini->value("table_width_4", 450).toInt());
        result.append(ini->value("table_width_5", 450).toInt());
        ini->endGroup();
        return result;
    }

    int tableHeight()
    {
        int result;
        ini->beginGroup("mainwindow");
        result = ini->value("table_height", 30).toInt();
        ini->endGroup();
        return result;
    }

    int createDataCount()
    {
        return 512;
    }

    int loadDataCount()
    {
        return 256;
    }

    int fontSize()
    {
        int result;
        ini->beginGroup("mainwindow");
        result = ini->value("font_size", 12).toInt();
        ini->endGroup();
        return result;
    }

    void setTableWidth(const QList<int> &width)
    {
        ini->beginGroup("mainwindow");
        ini->setValue("table_width_1", width.at(0));
        ini->setValue("table_width_2", width.at(1));
        ini->setValue("table_width_3", width.at(2));
        ini->setValue("table_width_4", width.at(3));
        ini->setValue("table_width_5", width.at(4));
        ini->endGroup();

    }

    void setTableHeight(const int &height)
    {
        ini->beginGroup("mainwindow");
        ini->setValue("table_height", height);
        ini->endGroup();
    }

    void setFontSize(const int &size)
    {
        ini->beginGroup("mainwindow");
        ini->setValue("font_size", size);
        ini->endGroup();
    }

}
