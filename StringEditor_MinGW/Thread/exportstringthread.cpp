#include "exportstringthread.h"

ExportStringThread::ExportStringThread(QObject *parent)
    : ProgressThread{parent} {}

ExportStringThread::ExportStringThread(const QString &export_folder, const QStringList &file_paths, const bool &filiter, QObject *parent)
    : ProgressThread(parent), paths(file_paths), is_filiter(filiter)
{
    // 导出所有文件
    if (paths.isEmpty()) json_path = export_folder + "/all.json";
    else json_path = export_folder + (is_filiter ? "/assic.json":"/not_assic.json");

    // 新建文件夹
    QDir dir(json_path);
    dir.mkpath(json_path.left(json_path.lastIndexOf('/')));
}

void ExportStringThread::run()
{
    Sql sql("EXPORT_JSON_THREAD");
    QFile file(json_path);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream stream(&file);

    QSqlQuery q = sql.sqlQuery("SELECT [FileInfo].file_path, [FileInfo].str_text, [StrInfo].cook_text FROM [FileInfo] "
                               "INNER JOIN [StrInfo] ON [FileInfo].str_text = [StrInfo].raw_text ", false);

    QString sql_source;
    QString sql_replace;
    bool first_write = true;
    QHash<QString, QString> texts;

    stream << '{' << '\n';
    while (q.next())
    {
        sql_source = q.value(1).toString();
        sql_replace = q.value(2).toString();

        if (!paths.isEmpty())
        {
            if (!paths.contains(q.value(0).toString())) continue;
            else if (sql_source.contains(QRegExp("[^\u0020-\u007E]+")) == is_filiter) continue;
        }

        if (texts.contains(sql_source)) continue;
        else texts[sql_source] = sql_replace;

        if (first_write) first_write = false;
        else stream << ",\n\n";
        stream << "    " << "\"" << sql_source << "\"" << ": \n";
        stream << "    " << "\"" << sql_replace << "\"";
    }
    stream << '\n' << '}' << '\n';
    file.close();
}
