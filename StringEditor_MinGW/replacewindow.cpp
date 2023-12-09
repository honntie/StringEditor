#include "replacewindow.h"
#include "ui_replacewindow.h"

ReplaceWindow::ReplaceWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReplaceWindow)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::Tool);

    //点击替换当前按钮函数
    connect(ui->replace_now_btn, &QPushButton::clicked, this, [=](){
        repalceNowString(ui->string_edit->text(), ui->now_edit->text());
    });

    //点击替换全部按钮函数
    connect(ui->replace_all_btn, &QPushButton::clicked, this, [=](){
        repalceAllString(ui->string_edit->text(), ui->now_edit->text());
    });
}

ReplaceWindow::~ReplaceWindow()
{
    delete ui;
}
