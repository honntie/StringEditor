#include "fileexportfilerunnable.h"

FileExportFileRunnable::FileExportFileRunnable(const QString &file, const QString &path) :
    HRunnable(), file_path(file), export_path(path) {}

void FileExportFileRunnable::run()
{
    QStringList file_text;    //文件文本

    /* 读取原文件 */
    QFile file(file_path);
    file.open(QIODevice::ReadOnly);
    QTextStream file_stream(&file);
    while (file_stream.atEnd() == false) file_text.append(file_stream.readLine());

    /* 导出文件 */
    QFile export_file(export_path);
    export_file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream export_stream(&export_file);

    QSqlQuery q = sql.sqlQuery(QString("SELECT [FileInfo].str_index,  [FileInfo].str_text, [StrInfo].cook_text FROM [FileInfo]"
                                    "INNER JOIN [StrInfo] ON [FileInfo].str_text = [StrInfo].raw_text "
                                    "WHERE [FileInfo].file_path LIKE '%1%' AND [StrInfo].cook_text != [FileInfo].str_text").arg(file_path), false);

    int now = 0;    //读取位置
    int line = q.next() ? q.value(0).toInt() - 1:file_text.length();    //当前数据库位置
    while (now < file_text.length())
    {
        /* 替换文本 */
        while (now == line)
        {
            file_text[now].replace(q.value(1).toString(), q.value(2).toString());
            qDebug() << file_text.at(now);
            line = q.next() ? q.value(0).toInt() - 1:file_text.length();
        }

        /* 导出文件 */
        for (; now < line; now++)
        {
            export_stream << file_text.at(now) << '\n';
            progressed(now, file_text.length());
        }
    }
}
