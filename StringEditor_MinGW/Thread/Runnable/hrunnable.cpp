#include "hrunnable.h"

HRunnable::HRunnable()
{
    setAutoDelete(true);
    /// 连接id不存在重复
    QString id;
    do id = QString::number(rand());
    while (QSqlDatabase::contains(id));
    sql.openDataBase(id);
}

HRunnable::~HRunnable()
{
    emit finished();
}
