#include <QApplication>
#include "mainwindow.h"
#include "replacewindow.h"

void test();    //测试|调试模块
void exportSqlData();    //导出过滤后的的文本
void exportSqlDict();    //导出过滤后的的字典
void txtFiliter(const QString &rich_file);    //过滤文本库

int main(int argc, char *argv[])
{
//    test(); return 0;

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
    IniSetting::connectIni("setting.ini");

//    QFile::remove(IniSetting::dataBase());

    QApplication a(argc, argv);

    QSqlDatabase db = Sql::connectDataBase();
    int i = 0;
     while (db.isOpen() == false && i == 0)
        i = QMessageBox::information(nullptr, "sqlite error", db.lastError().text(), "重试", "退出");
     if (i == 1) return 0;
     else
     {
         Sql::exec("CREATE TABLE IF NOT EXISTS [OpenInfo]("
                   "open_path      NTEXT       PRIMARY KEY,"
                   "is_open        BOOL        NOT NULL,"
                   "last_modifie   DATETIME    NOT NULL)");

         Sql::exec("CREATE TABLE IF NOT EXISTS [FileInfo]("
                   "file_path      NTEXT       NOT NULL,"
                   "file_info      VARCHAR(2)  NOT NULL,"
                   "str_index      LONG        NOT NULL,"
                   "str_text       NTEXT       NOT NULL)");

         Sql::exec("CREATE TABLE IF NOT EXISTS [StrInfo]("
                   "raw_text       NTEXT       PRIMARY KEY,"
                   "cook_text      NTEXT       NTEXT)");
     }

//    exportSqlDict();
//     return 0;

//    exportSqlData();
//    txtFiliter("Simplified_Chinese.txt");
//    return 0;

    MainWindow w;
    w.show();
    return a.exec();
}

QStringList getAllFilePath(const QString &dir_path)
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
        all_path.append(getAllFilePath(folder_path));
    }

    /// 目录下文件
    for (QString short_path : file_paths)
    {
        file_path = dir_path + "/" + short_path;
        all_path.append(file_path);
    }
    return all_path;
}

void test()
{
    QString path = "D:\\Desktop\\text.json";
    QFile file(path);
    QTextStream stream(&file);
    if (!file.open((QIODevice::ReadOnly) | QIODevice::Text)) return;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    if (!doc.isObject()) return;

    QVariantMap json = doc.object().toVariantMap();
    qDebug() << json.size();
}

void txtFiliter(const QString &rich_file)
{
    QStringList codes;
    QFile text(rich_file);
    text.open(QIODevice::ReadOnly);
    QTextStream text_stream(&text);
    while (text_stream.atEnd() == false)
    {
        QString code = text.readAll();
        for (int i = 0; i < code.length(); i++)
            codes.append(code.at(i));
    }

    QFile read("cook.txt");
    read.open(QIODevice::ReadOnly);
    QTextStream read_stream(&read);

    QString export_text;
    QFile export_file("export.txt");
    export_file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream export_stream(&export_file);
    while (read_stream.atEnd() == false)
    {
        QString string = read_stream.readLine();
        for (int i = 0; i < string.length(); i++)
        {
            QString code = string.at(i);
            if (codes.contains(code) == false)
            {
                if (export_text.contains(code)) continue;
                export_stream << code;
                export_text.append(code);
            }
        }
    }
}

void exportSqlData()
{
    QFile f("cook.txt");
    f.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream s(&f);
    QSqlQuery q = Sql::query("SELECT * FROM [StrInfo] WHERE raw_text REGEXP '[^\u0020-\u007E]+'", false);
    while (q.next()) s << q.value(0).toString() << '\n';
}

void exportSqlDict()
{
    QFile f("cook.cs");
    f.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream s(&f);
    s << "{" << '\n';
    QSqlQuery q = Sql::query("SELECT * FROM [StrInfo] WHERE raw_text REGEXP '[^\u0020-\u007E]+'", false);
//    QSqlQuery q = Sql::query("SELECT * FROM [StrInfo] WHERE raw_text REGEXP '[^\u0020-\u007E]+' AND raw_text != cook_text", false);
    while (q.next())
    {
        s << "\t" << "{" << "\n";
        s << "\t\t" << "\"" << q.value(0).toString() << "\"" << ",\n";
        s << "\t\t" << "\"" << q.value(1).toString() << "\"" << "\n";
        s << "\t" << "}, " << "\n";
    }
    s << "}" << '\n';
}
