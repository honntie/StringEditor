#include "importstringthread.h"

ImportStringThread::ImportStringThread(QObject *parent)
    : ProgressThread(parent) {}

ImportStringThread::ImportStringThread(const QString &json_path, QObject *parent)
    : ProgressThread(parent), path(json_path) {}

void ImportStringThread::run()
{
    QFile file(path);
    if (!file.open((QIODevice::ReadOnly))) return;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    if (!doc.isObject()) return;

    QVariantMap json = doc.object().toVariantMap();
    Sql sql("OPEN_JSON_THREAD");
    int i = 0;
    QString key, value;
    json_count = json.size();
    sql.sqlTransaction();
    emit progressed(i, json_count);
    for (QVariantMap::Iterator it = json.begin(); it != json.end(); it++)
    {
        key = it.key();
        value = it.value().toString();
        sql.sqlExec("INSERT INTO [StrInfo] SELECT :1, :2 WHERE NOT EXISTS (SELECT * FROM [StrInfo] WHERE raw_text = :1)", {key.replace('\n', "\\n"), value.replace('\n', "\\n")});
        sql.sqlExec("UPDATE [StrInfo] SET cook_text = :2 WHERE raw_text = :1", {key, value});
        emit progressed(++i, json_count);
    }
    sql.sqlCommit();
}

int ImportStringThread::GetStringCount() const
{
    return json_count;
}
