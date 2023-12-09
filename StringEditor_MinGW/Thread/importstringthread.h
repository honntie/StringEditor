#ifndef IMPORTSTRINGTHREAD_H
#define IMPORTSTRINGTHREAD_H

#include "progressthread.h"
#include "Handle/sql.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

class ImportStringThread : public ProgressThread
{
private:
    QString path;
    int json_count = 0;

public:
    explicit ImportStringThread(QObject *parent = nullptr);
    ImportStringThread(const QString &json_path, QObject *parent = nullptr);
    void run() override;
    int GetStringCount() const;
};

#endif // IMPORTSTRINGTHREAD_H
