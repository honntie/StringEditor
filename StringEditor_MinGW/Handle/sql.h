#ifndef SQL_H
#define SQL_H


#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlError>
#include <QDebug>
#include "iniseting.h"

#define ERROR true    //是否输出调试

class Sql
{
private:
    QSqlDatabase db;
    static bool is_debug;

public:
    Sql();
    Sql(const QString &name);
    bool openDataBase(const QString &name = QString());    //打开数据库
    ~Sql();
    bool sqlTransaction();    //开始事务
    bool sqlCommit();    //结束事务
    void close();    //关闭数据库

    /// 函数功能: 链接数据库
    /// name: 链接数据库的连接名, 默认为空
    /// 返回: 打开后的QSqlDatabase, 无论打开是否成功
    static QSqlDatabase connectDataBase(const QString &name = QString());


    /*
     *
     *
     * 类中使用函数, 可用于多线程执行
     *
     *
     */

    /// 函数功能: 执行语句
    /// sql_query: 执行的数据库语句
    /// 重载参数values: 替换对应的[:数字], 数字1为起点
    /// 返回: 是否执行成功
    bool sqlExec(const QString &sql_query);
    bool sqlExec(const QString &sql_query, const QVariantList &values);    //执行语句

    /// 函数功能: 查询语句
    /// sql_query: 执行的数据库语句
    /// is_fir: 执行完后是否跳转, true为跳转到第一行的数据, false不跳转
    /// 重载参数values: 替换对应的[:数字], 数字1为起点
    /// 返回: 执行完后的QSqlQuery
    QSqlQuery sqlQuery(const QString &sql_query, const bool &is_fir = true);    //
    QSqlQuery sqlQuery(const QString &sql_query, const QVariantList &values, const bool &is_fir = true);

    /// 函数功能: 查询存在
    /// sql_query: 执行的数据库语句
    /// 重载参数values: 替换对应的[:数字], 数字1为起点
    /// 返回: 执行后是否有返回数据
    bool sqlIsExist(const QString &sql_query);
    bool sqlIsExist(const QString &sql_query, const QVariantList &values);


    /*
     *
     *
     * 类外使用函数, 直接执行
     *
     *
     */


    static bool transaction();    //开始事务
    static bool commit();    //结束事务

    /// 函数功能: 执行语句
    /// sql_query: 执行的数据库语句
    /// 重载参数values: 替换对应的[:数字], 数字1为起点
    /// 返回: 是否执行成功
    static bool exec(const QString &sql_query);    //执行语句
    static bool exec(const QString &sql_query, const QVariantList &values);    //执行语句

    /// 函数功能: 查询语句
    /// sql_query: 执行的数据库语句
    /// is_fir: 执行完后是否跳转, true为跳转到第一行的数据, false不跳转
    /// 重载参数values: 替换对应的[:数字], 数字1为起点
    /// 返回: 执行完后的QSqlQuery
    static QSqlQuery query(const QString &sql_query, const bool &is_fir = true);    //查询语句
    static QSqlQuery query(const QString &sql_query, const QVariantList &values, const bool &is_fir = true);    //查询语句

    /// 函数功能: 查询存在
    /// sql_query: 执行的数据库语句
    /// 重载参数values: 替换对应的[:数字], 数字1为起点
    /// 返回: 执行后是否有返回数据
    static bool isExist(const QString &sql_query);    //查询存在
    static bool isExist(const QString &sql_query, const QVariantList &values);    //查询存在
};

#endif // SQL_H
