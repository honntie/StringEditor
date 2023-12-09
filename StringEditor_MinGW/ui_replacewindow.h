/********************************************************************************
** Form generated from reading UI file 'replacewindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REPLACEWINDOW_H
#define UI_REPLACEWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ReplaceWindow
{
public:
    QGridLayout *gridLayout;
    QLineEdit *string_edit;
    QPushButton *replace_now_btn;
    QLineEdit *now_edit;
    QPushButton *replace_all_btn;

    void setupUi(QDialog *ReplaceWindow)
    {
        if (ReplaceWindow->objectName().isEmpty())
            ReplaceWindow->setObjectName(QString::fromUtf8("ReplaceWindow"));
        ReplaceWindow->resize(274, 68);
        gridLayout = new QGridLayout(ReplaceWindow);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        string_edit = new QLineEdit(ReplaceWindow);
        string_edit->setObjectName(QString::fromUtf8("string_edit"));

        gridLayout->addWidget(string_edit, 0, 0, 1, 1);

        replace_now_btn = new QPushButton(ReplaceWindow);
        replace_now_btn->setObjectName(QString::fromUtf8("replace_now_btn"));

        gridLayout->addWidget(replace_now_btn, 0, 1, 1, 1);

        now_edit = new QLineEdit(ReplaceWindow);
        now_edit->setObjectName(QString::fromUtf8("now_edit"));

        gridLayout->addWidget(now_edit, 1, 0, 1, 1);

        replace_all_btn = new QPushButton(ReplaceWindow);
        replace_all_btn->setObjectName(QString::fromUtf8("replace_all_btn"));

        gridLayout->addWidget(replace_all_btn, 1, 1, 1, 1);


        retranslateUi(ReplaceWindow);

        QMetaObject::connectSlotsByName(ReplaceWindow);
    } // setupUi

    void retranslateUi(QDialog *ReplaceWindow)
    {
        ReplaceWindow->setWindowTitle(QCoreApplication::translate("ReplaceWindow", "\346\233\277\346\215\242", nullptr));
        string_edit->setPlaceholderText(QCoreApplication::translate("ReplaceWindow", "\350\242\253\346\233\277\346\215\242...", nullptr));
        replace_now_btn->setText(QCoreApplication::translate("ReplaceWindow", "\346\233\277\346\215\242\345\275\223\345\211\215", nullptr));
        now_edit->setPlaceholderText(QCoreApplication::translate("ReplaceWindow", "\346\233\277\346\215\242\346\210\220...", nullptr));
        replace_all_btn->setText(QCoreApplication::translate("ReplaceWindow", "\346\233\277\346\215\242\345\205\250\351\203\250", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ReplaceWindow: public Ui_ReplaceWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REPLACEWINDOW_H
