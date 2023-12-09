#ifndef REPLACEWINDOW_H
#define REPLACEWINDOW_H

#include <QDialog>

namespace Ui {
class ReplaceWindow;
}

class ReplaceWindow : public QDialog
{
    Q_OBJECT
private:
    Ui::ReplaceWindow *ui;

private slots:

signals:
    void repalceNowString(const QString &, const QString &);    //替换当前字符串
    void repalceAllString(const QString &, const QString &);    //替换全部字符串

public:
    explicit ReplaceWindow(QWidget *parent = 0);
    ~ReplaceWindow();

};

#endif // REPLACEWINDOW_H
