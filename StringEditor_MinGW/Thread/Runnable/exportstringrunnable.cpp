#include "exportstringrunnable.h"

ExportStringRunnable::ExportStringRunnable(int id, QString user_file) :
    thread_id(id), file_path(user_file)
{
    setAutoDelete(true);
}

void ExportStringRunnable::run()
{
    /**************** 线程数据库 ****************/
    Sql sql(QString::number(thread_id));

    /**************** 打开文件 ****************/
    QFile f(file_path);
    f.open(QIODevice::ReadOnly);
    QTextStream t(&f);

    /**************** 筛选加入数据 ****************/
    QList<QVariantList> add_data_list;
    int line = 0;
    while (t.atEnd() == false)
    {
        line++;
        QString text = t.readLine();
        while (text[0] == "\t" || text[0] == " ") text.remove(0,1);
        /**************** 包含\"的文本 ****************/
        while (text.count("\"") >= 2)
        {
            /**************** 字符串位置长度 ****************/
            int fir_index = text.indexOf("\"") + 1;
            int sec_index = text.indexOf("\"", fir_index);
            int str_len = sec_index - fir_index;

            if (sec_index == -1 || fir_index == 0) break;
            QString str_txt = text.mid(fir_index, str_len);

            /**************** 数据库不存在该数据则加入数据库 ****************/
            if (sql.sqlIsExist("SELECT * FROM show_info WHERE file_path = :1 AND line = :2 AND file_text = :3 AND string_text = :4",
                {file_path, line, text, str_txt}) == false) add_data_list.append({file_path, line, text, str_txt});
            text.remove("\""+str_txt+"\"");
        }
    }
    f.close();

    /**************** 开始加入数据 ****************/
    sql.startTransaction();
    for (QVariantList values : add_data_list)
    {
        /**************** 正则过滤纯数字，英文，符号的组合 ****************/
        QString symbol = "\u0021-\u002F\u003A-\u0040\u005B-\u0060\u007B-\u007E\u0009\u000B";
        QString num = "0-9";
        QString letter = "a-zA-Z";
        QRegExp r("^["+symbol+num+letter+"\u0020]+$");

        QString str_txt = values.at(3).toString();
        sql.sqlExec(QString("insert into %1_info values(:1,:2,:3,:4,:4)").arg((str_txt.contains(r) || str_txt.isEmpty()) ? "hidden" : "show"), values);
    }
    emit finished();
}
