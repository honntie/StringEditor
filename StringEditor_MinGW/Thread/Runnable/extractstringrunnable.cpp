#include "extractstringrunnable.h"

ExtractStringRunnable::ExtractStringRunnable(const QString &open_file) :
     HRunnable(), path(open_file) {}

void ExtractStringRunnable::run()
{
    /// 打开文件
    QFile f(path);
    f.open(QIODevice::ReadOnly);
    QTextStream s(&f);

    /// 保存所有文本
    QStringList all_text;    //所有文本，用于显示进度
    while (s.atEnd() == false) all_text.append(s.readLine());
    f.close();

    /// 逐行读取截取字符串
    int fir_index;    //第一个引号
    int sec_index;    //第二个引号
    QList<int> all_index;    //字符串位置
    QStringList all_str;    //所有字符串
    int str_len;    //字符串长度
    QString str_txt;    //字符串文本

    for (int line = 0; line < all_text.length(); line++)
    {
        QString text = all_text.at(line);

        /// 寻找字符串
        while (text.count("\"") >= 2)
        {
            /// 字符串开始位置
            fir_index = text.indexOf("\"") + 1;
            if (fir_index == 0) break;

            /// 字符串结束位置并跳过 \" 符号
            sec_index = fir_index - 1;
            do
            {
                sec_index = text.indexOf("\"", sec_index + 1);
                if (sec_index == -1) break;
            }
            while (text.at(sec_index - 1) == '\\');
            if (sec_index == -1) break;

            ///获取字符串
            str_len = sec_index - fir_index;
            if (str_len == 0) break;
            str_txt = text.mid(fir_index, str_len);

            /// 一会使用事务写入
            all_index.append(line + 1);
            all_str.append(str_txt);
            text.remove("\"" + str_txt + "\"");
        }
        emit progressed(line, all_text.length());
    }

    /// 写入数据库
    sql.sqlTransaction();
    for (int i = 0; i < all_str.length(); i++)
    {
        str_txt = all_str.at(i);
        sql.sqlExec("INSERT INTO [FileInfo] VALUES(:1, :2, :3, :4)", {path, FILE, all_index.at(i), str_txt});
        sql.sqlExec("INSERT INTO [StrInfo] SELECT :1, :1 WHERE NOT EXISTS (SELECT * FROM [StrInfo] WHERE raw_text = :1)", {str_txt});
    }
}
