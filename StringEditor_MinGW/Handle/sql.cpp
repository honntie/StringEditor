#include "sql.h"

bool Sql::is_debug = true;

Sql::Sql() {}

Sql::Sql(const QString &name)
{
    openDataBase(name);
}

bool Sql::openDataBase(const QString &name)
{
    db = connectDataBase(name);
    return db.isOpen();
}

Sql::~Sql()
{
    close();
}

bool Sql::sqlTransaction()
{
    return db.transaction();
}

bool Sql::sqlCommit()
{
    return db.commit();
}

void Sql::close()
{
    db.commit();
    while (db.isOpen()) db.close();
    while (db.isValid()) QSqlDatabase::removeDatabase(db.connectionName());
}

bool Sql::sqlExec(const QString &sql_query)
{
    QSqlQuery q(db);
    bool is_exec = q.exec(sql_query);
    if (is_exec == false && is_debug)
    {
        qDebug() << "error from:" << sql_query;
        qDebug() <<q.lastError().text();
    }
    return is_exec;
}

bool Sql::sqlExec(const QString &sql_query, const QVariantList &values)
{
    QSqlQuery q(db);
    q.prepare(sql_query);
    for (int i = 0; i < values.length(); i++)
        q.bindValue(":" + QString::number(i + 1), values.at(i));
    bool is_exec = q.exec();
    if (is_exec == false && is_debug)
    {
        qDebug() << "error from:" << sql_query;
        qDebug() <<q.lastError().text();
    }
    return is_exec;
}

QSqlQuery Sql::sqlQuery(const QString &sql_query, const bool &is_fir)
{
    QSqlQuery q(db);
    bool is_exec = q.exec(sql_query);
    if (is_exec == false && is_debug)
    {
        qDebug() << "error from:" << sql_query;
        qDebug() <<q.lastError().text();
    }
    if (is_fir) q.first();
    return q;
}

QSqlQuery Sql::sqlQuery(const QString &sql_query, const QVariantList &values, const bool &is_fir)
{
    QSqlQuery q(db);
    q.prepare(sql_query);
    for (int i = 0; i < values.length(); i++)
        q.bindValue(":" + QString::number(i + 1), values.at(i));
    bool is_exec = q.exec();
    if (is_exec == false && is_debug)
    {
        qDebug() << "error from:" << sql_query;
        qDebug() <<q.lastError().text();
    }
    if (is_fir) q.first();
    return q;
}

bool Sql::sqlIsExist(const QString &sql_query)
{
    QSqlQuery q(db);
    bool is_exec = q.exec(sql_query);
    if (is_exec == false && is_debug)
    {
        qDebug() << "error from:" << sql_query;
        qDebug() <<q.lastError().text();
    }
    return q.first();
}

bool Sql::sqlIsExist(const QString &sql_query, const QVariantList &values)
{
    QSqlQuery q(db);
    q.prepare(sql_query);
    for (int i = 0; i < values.length(); i++)
        q.bindValue(":" + QString::number(i + 1), values.at(i));
    bool is_exec = q.exec();
    if (is_exec == false && is_debug)
    {
        qDebug() << "error from:" << sql_query;
        qDebug() <<q.lastError().text();
    }
    return q.first();
}

bool Sql::transaction()
{
    return QSqlDatabase::database().transaction();
}

bool Sql::commit()
{
    return QSqlDatabase::database().commit();
}

/*
 *
 *
 *
 *下面为static函数
 *
 *
 *
 */

QSqlDatabase Sql::connectDataBase(const QString &name)
{
    if (QSqlDatabase::contains(name)) return QSqlDatabase::database(name);
    QSqlDatabase data_base = name.isEmpty() ? QSqlDatabase::addDatabase("QSQLITE"):QSqlDatabase::addDatabase("QSQLITE", name);
    data_base.setDatabaseName(IniSetting::dataBase());
    data_base.setConnectOptions("QSQLITE_ENABLE_REGEXP");
    if (data_base.open())
    {
        data_base.exec("PRAGMA synchronous = OFF");
        data_base.exec("PRAGMA journal_mode = MEMORY");
        data_base.exec("PRAGMA cache_size = 21876453");
        data_base.exec("PRAGMA temp_store = MEMORY");
        data_base.exec("PRAGMA case_sensitive_like = 1");
    }
    return data_base;
}

bool Sql::exec(const QString &sql_query)
{
    QSqlQuery q;
    bool is_exec = q.exec(sql_query);
    if (is_exec == false && is_debug)
    {
        qDebug() << "error from:" << sql_query;
        qDebug() <<q.lastError().text();
    }
    return is_exec;
}

bool Sql::exec(const QString &sql_query, const QVariantList &values)
{
    QSqlQuery q;
    q.prepare(sql_query);
    for (int i = 0; i < values.length(); i++)
        q.bindValue(":" + QString::number(i + 1), values.at(i));
    bool is_exec = q.exec();
    if (is_exec == false && is_debug)
    {
        qDebug() << "error from:" << sql_query;
        qDebug() <<q.lastError().text();
    }
    return is_exec;
}

QSqlQuery Sql::query(const QString &sql_query, const bool &is_fir)
{
    QSqlQuery q;
    bool is_exec = q.exec(sql_query);
    if (is_exec == false && is_debug)
    {
        qDebug() << "error from:" << sql_query;
        qDebug() <<q.lastError().text();
    }
    if (is_fir) q.first();
    return q;
}

QSqlQuery Sql::query(const QString &sql_query, const QVariantList &values, const bool &is_fir)
{
    QSqlQuery q;
    q.prepare(sql_query);
    for (int i = 0; i < values.length(); i++)
        q.bindValue(":" + QString::number(i + 1), values.at(i));
    bool is_exec = q.exec();
    if (is_exec == false && is_debug)
    {
        qDebug() << "error from:" << sql_query;
        qDebug() <<q.lastError().text();
    }
    if (is_fir) q.first();
    return q;
}

bool Sql::isExist(const QString &sql_query)
{
    QSqlQuery q;
    bool is_exec = q.exec(sql_query);
    if (is_exec == false && is_debug)
    {
        qDebug() << "error from:" << sql_query;
        qDebug() <<q.lastError().text();
    }
    return q.first();
}

bool Sql::isExist(const QString &sql_query, const QVariantList &values)
{
    QSqlQuery q;
    q.prepare(sql_query);
    for (int i = 0; i < values.length(); i++)
        q.bindValue(":" + QString::number(i + 1), values.at(i));
    bool is_exec = q.exec();
    if (is_exec == false && is_debug)
    {
        qDebug() << "error from:" << sql_query;
        qDebug() <<q.lastError().text();
    }
    return q.first();
}
