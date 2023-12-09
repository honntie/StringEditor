#include "filecontroltread.h"

FileControlTread::FileControlTread(QObject *parent) :
     QThread(parent)
{
    sql.openDataBase("FILE_THREAD");
    connect(this, &FileControlTread::finished, this, &FileControlTread::deleteLater);
    finish_count = 0;
}

void FileControlTread::run()
{
    /* 线程池初始化 */
    QThreadPool pool;
    int thread_count = IniSetting::threadCount();
    if (thread_count <= 0) thread_count = QThread::idealThreadCount();
    pool.setMaxThreadCount(thread_count + 2);

    switch (mode)
    {
        /// 打开普通文件
        case OPEN_FILE:
        {
            ExtractStringRunnable *runnable = new ExtractStringRunnable(path);
            connect(runnable, &ExtractStringRunnable::progressed, this, &FileControlTread::progressed);
            pool.start(runnable);
            break;
        }

        /// 打开json文件
//        case OPEN_JSON:
//            break;

        /// 打开文件夹
        case OPEN_FOLDER:
        {
            for (const QString &file_path : paths)
            {
                HRunnable *runnable = getFileRunnable(file_path);
                connectRunnableProgressed(runnable);
                pool.start(runnable);
            }
            break;
        }

        /// 打开meat-data文件
//        case OPEN_MEAT:
//        break;

        /// 导出文件
        case EXPORT_FILE:
        {
            ExportFileRunnable *runnable = new ExportFileRunnable(path_map.firstKey(), path_map.value(path_map.firstKey()));
            emit progressed(0, 1);
            pool.start(runnable);
            break;
        }

        /// 导出多个文件
        case EXPORT_FOLDER:
        {
            for (const QString &path : path_map.keys())
            {
                ExportFileRunnable *runnable = new ExportFileRunnable(path, path_map.value(path));
                emit progressed(0, path_map.size());
                connect(runnable, &ExportFileRunnable::finished, this, [=](){
                    progressed(++finish_count, path_map.size());
                });
                pool.start(runnable);
            }
            break;
        }

        /// 导出json文件
        case EXPORT_JSON:
        {
            QFile file(path + "/word.json");
            file.open(QIODevice::WriteOnly | QIODevice::Truncate);
            QTextStream stream(&file);

            QString filiter_paths;
            for (const QString filiter_path : paths)
                filiter_paths +=  "\'" + filiter_path + "\', ";
            filiter_paths.remove(filiter_paths.length() - 2, 1);

            QSqlQuery q = sql.sqlQuery(QString("SELECT [FileInfo].str_text, [StrInfo].cook_text FROM [FileInfo] "
                                   "INNER JOIN [StrInfo] ON [FileInfo].str_text = [StrInfo].raw_text %1 "
                                   "GROUP BY [FileInfo].str_text"
                                   ).arg(paths.isEmpty() ? "" : QString("WHERE [FileInfo].file_path IN (%1)").arg(filiter_paths)), false);
            stream << '{' << '\n';
            if  (q.first())
            {
                while (true)
                {
                    stream << "    " << "\"" << q.value(0).toString() << "\"" << ": \n";
                    stream << "    " << "\"" << q.value(1).toString() << "\"";
                    if (q.next()) stream << ",\n\n";
                    else break;
                }
            }
            stream << '}' << '\n';
            file.close();
            break;
        }
    }
    pool.waitForDone();
}

QString FileControlTread::getPath() const
{
    return path;
}

void FileControlTread::openFileRun(const QString &open_path)
{
    path = open_path;
    QFileInfo info(path);
    if (info.isFile() == false)
    {
        mode = OPEN_FOLDER;
        paths = getAllPath(open_path);
    }
    else if (info.fileName() == "global-metadata.dat") mode = OPEN_MEAT;
    else if (info.suffix() == "json") mode = OPEN_JSON;
    else mode = OPEN_FILE;
    start();
}

void FileControlTread::exportFileRun(const QMap<QString, QString> &map)
{
    if (map.isEmpty()) return;
    mode = map.keys().length() < 2 ? EXPORT_FILE : EXPORT_FOLDER;
    path_map = map;
    start();
}

void FileControlTread::exportJsonRun(const QString &export_folder, const QStringList &files)
{
    path = export_folder;
    mode = EXPORT_JSON;
    paths = files;
    start();
}

float FileControlTread::getSmallProgress()
{
    float num = 0;
    for (float n : runnable_progress.values()) num += n;
    return num;
}

QStringList FileControlTread::getAllPath(const QString &dir_path) const
{
    QStringList all_path;
    QDir dir(dir_path);
    QString folder_path;    //文件夹路径
    QString file_path;    //文件路径
    QStringList folder_paths = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList file_paths = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);

    /// 目录下文件夹下文件
    for (QString short_path : folder_paths)
    {
        folder_path = dir_path + "/" + short_path;
        all_path.append(getAllPath(folder_path));
    }

    /// 目录下文件
    for (QString short_path : file_paths)
    {
        file_path = dir_path + "/" + short_path;
        all_path.append(file_path);
    }
    return all_path;
}

HRunnable *FileControlTread::getFileRunnable(const QString &file_path)
{
    QFileInfo info(file_path);

    /// 文件类型错误
    if (info.isFile() == false) return nullptr;

    /// meat-data文件类型
//    else if (info.fileName() == "global-metadata.dat") return new QRunnable();

    /// json文件类型 (可能不需要)
//    else if (info.suffix() == "json") return new QRunnable();

    /// 普通文件类型
    else return new ExtractStringRunnable(file_path);
}

void FileControlTread::connectRunnableProgressed(HRunnable *runnable)
{
    connect(runnable, &HRunnable::progressed, this, [=](const int& a, const int& b){
        if (b > 0) runnable_progress[runnable] = (a / (float)b);
        emit progressed(finish_count + getSmallProgress(), paths.length());
    });
    connect(runnable, &HRunnable::finished, this, [=](){
        runnable_progress.remove(runnable);
        emit progressed(++finish_count + getSmallProgress(), paths.length());
    });
}
