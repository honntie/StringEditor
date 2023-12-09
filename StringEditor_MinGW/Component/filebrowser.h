#ifndef FILEBROWSER_H
#define FILEBROWSER_H

#include <QPlainTextEdit>
#include <QTextBlock>
#include <QDebug>
#include <QThread>

class FileBrowser : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit FileBrowser(QWidget *parent = nullptr);
    ~FileBrowser();

public slots:
    QString getPath() const;
    void setFilePath(const QString &path);
    void setHighlight(const QString &path,const int &index, QString string);

private:
    QString file_path;
    QString last_highlight;

//    void highLightString();

signals:
    void readyLoadFile();
};

#endif // FILEBROWSER_H
