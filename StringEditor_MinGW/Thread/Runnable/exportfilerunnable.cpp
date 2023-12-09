#include "exportfilerunnable.h"

ExportFileRunnable::ExportFileRunnable(QString source_file, QString export_file) :
    HRunnable(), file_path(source_file), export_path(export_file)
{
    QDir dir(export_file);
    dir.mkpath(export_file.left(export_file.lastIndexOf('/')));
}

void ExportFileRunnable::run()
{
    QFile source_file(file_path);
    source_file.open(QIODevice::ReadOnly);
    QTextStream source_stream(&source_file);
    QStringList source_text;
    while (source_file.atEnd() == false) source_text.append(source_file.readLine());

    QSqlQuery q = sql.sqlQuery(QString("SELECT [FileInfo].str_index, [FileInfo].str_text, [StrInfo].cook_text FROM [FileInfo] "
                                    "INNER JOIN [StrInfo] ON [FileInfo].str_text = [StrInfo].raw_text "
                                    "WHERE [FileInfo].file_path = '%1' AND "
                                          "[StrInfo].cook_text != [FileInfo].str_text "
                                    "ORDER BY [FileInfo].str_index ASC").arg(file_path), false);
    QString source;    //源字符串
    QString replace;    //修改后字符串
    qlonglong index;    //行数位置
    while (q.next())
    {
        source = '\"' + q.value(1).toString() + '\"';
        replace = '\"' + q.value(2).toString() + '\"';
        index = q.value(0).toLongLong() - 1;
        source_text[index] = source_text[index].replace(source, replace);
    }

    QFile export_file(export_path);
    export_file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream export_stream(&export_file);
    for (QString text : source_text) export_stream << text;
}
