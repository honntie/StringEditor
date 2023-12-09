#include "filebrowser.h"

FileBrowser::FileBrowser(QWidget *parent) :
    QPlainTextEdit(parent)
{
    QFontMetrics metrics(font());
    setTabStopDistance(4 * metrics.horizontalAdvance(' '));
}

FileBrowser::~FileBrowser()
{
}

QString FileBrowser::getPath() const
{
    return file_path;
}

void FileBrowser::setFilePath(const QString &path)
{
    if (file_path == path)
    {
        if (last_highlight.isEmpty()) return;
        /* 清除高亮 */
        QTextCursor highlight_cursor(document());
        QTextCursor cursor(document());
        QTextCharFormat color_format(highlight_cursor.charFormat());
        color_format.setForeground(Qt::black);
        color_format.setBackground(Qt::transparent);

        cursor.beginEditBlock();
        while (!highlight_cursor.isNull() && !highlight_cursor.atEnd())
        {
            highlight_cursor = document()->find(last_highlight, highlight_cursor, QTextDocument::FindWholeWords);
            if (!highlight_cursor.isNull())  highlight_cursor.mergeCharFormat(color_format);
        }
        cursor.endEditBlock();
        last_highlight.clear();
    }
    else
    {
        document()->clear();
        /* 读取文件 */
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        QTextStream stream(&file);
        QString text;

        while (stream.atEnd() == false) text.append(stream.readLine() + '\n');
        setPlainText(text);

        file_path = path;
    }
}

void FileBrowser::setHighlight(const QString &path, const int &index, QString string)
{
    if (file_path == path)
    {
        /* 清除高亮 */
        QTextCursor highlight_cursor(document());
        QTextCursor cursor(document());
        QTextCharFormat color_format(highlight_cursor.charFormat());
        color_format.setForeground(Qt::black);
        color_format.setBackground(Qt::transparent);

        cursor.beginEditBlock();
        while (!highlight_cursor.isNull() && !highlight_cursor.atEnd())
        {
            highlight_cursor = document()->find(last_highlight, highlight_cursor, QTextDocument::FindWholeWords);
            if (!highlight_cursor.isNull())  highlight_cursor.mergeCharFormat(color_format);
        }
        cursor.endEditBlock();
    }
    else
    {
        /* 读取文件 */
        document()->clear();
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        QTextStream stream(&file);
        QString text;

        while (stream.atEnd() == false) text.append(stream.readLine() + '\n');
        setPlainText(text);

        file_path = path;
    }

    /* 显示高亮 */
    string = '\"' + string + '\"';
    QTextCursor highlight_cursor(document());
    QTextCursor cursor(document());
    QTextCharFormat color_format(highlight_cursor.charFormat());
    color_format.setForeground(Qt::white);
    color_format.setBackground(Qt::blue);

    cursor.beginEditBlock();
    while (!highlight_cursor.isNull() && !highlight_cursor.atEnd())
    {
        highlight_cursor = document()->find(string, highlight_cursor, QTextDocument::FindWholeWords);
        if (!highlight_cursor.isNull()) highlight_cursor.mergeCharFormat(color_format);
    }
    cursor.endEditBlock();

    /* 跳转位置 */
    cursor = textCursor();
    int position = document()->findBlockByNumber(index - 1).position();
    cursor.setPosition(position, QTextCursor::MoveAnchor);
    cursor = document()->find(string, cursor);
    setTextCursor(cursor);
    last_highlight = string;
    setFocus();
}
