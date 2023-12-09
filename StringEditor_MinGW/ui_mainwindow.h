/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "./Component/filebrowser.h"
#include "./Component/filetreewidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *open_file_action;
    QAction *open_json_action;
    QAction *open_folder_action;
    QAction *export_checked_files_action;
    QAction *export_all_files_action;
    QAction *export_checked_json_action;
    QAction *export_now_json_action;
    QAction *export_all_json_action;
    QAction *show_preview_action;
    QAction *show_hideen_data_action;
    QAction *show_not_tran_action;
    QAction *show_same_action;
    QAction *undo_action;
    QAction *resume_aciton;
    QAction *replace_action;
    QAction *show_source_action;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_2;
    QTabWidget *open_file_tables;
    QDockWidget *file_manage;
    QWidget *dockWidgetContents;
    QHBoxLayout *horizontalLayout;
    FileTreeWidget *files_tree_manage;
    QDockWidget *preview_manage;
    QWidget *dockWidgetContents_2;
    QHBoxLayout *horizontalLayout_3;
    FileBrowser *file_preview_text;
    QToolBar *status_bar;
    QMenuBar *menubar;
    QMenu *file_meun;
    QMenu *edit_menu;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1440, 800);
        open_file_action = new QAction(MainWindow);
        open_file_action->setObjectName(QString::fromUtf8("open_file_action"));
        open_json_action = new QAction(MainWindow);
        open_json_action->setObjectName(QString::fromUtf8("open_json_action"));
        open_json_action->setEnabled(true);
        open_folder_action = new QAction(MainWindow);
        open_folder_action->setObjectName(QString::fromUtf8("open_folder_action"));
        export_checked_files_action = new QAction(MainWindow);
        export_checked_files_action->setObjectName(QString::fromUtf8("export_checked_files_action"));
        export_all_files_action = new QAction(MainWindow);
        export_all_files_action->setObjectName(QString::fromUtf8("export_all_files_action"));
        export_checked_json_action = new QAction(MainWindow);
        export_checked_json_action->setObjectName(QString::fromUtf8("export_checked_json_action"));
        export_checked_json_action->setEnabled(true);
        export_now_json_action = new QAction(MainWindow);
        export_now_json_action->setObjectName(QString::fromUtf8("export_now_json_action"));
        export_now_json_action->setEnabled(true);
        export_all_json_action = new QAction(MainWindow);
        export_all_json_action->setObjectName(QString::fromUtf8("export_all_json_action"));
        export_all_json_action->setEnabled(true);
        show_preview_action = new QAction(MainWindow);
        show_preview_action->setObjectName(QString::fromUtf8("show_preview_action"));
        show_preview_action->setCheckable(true);
        show_preview_action->setChecked(true);
        show_preview_action->setEnabled(true);
        show_hideen_data_action = new QAction(MainWindow);
        show_hideen_data_action->setObjectName(QString::fromUtf8("show_hideen_data_action"));
        show_hideen_data_action->setCheckable(true);
        show_hideen_data_action->setChecked(false);
        show_not_tran_action = new QAction(MainWindow);
        show_not_tran_action->setObjectName(QString::fromUtf8("show_not_tran_action"));
        show_not_tran_action->setCheckable(true);
        show_same_action = new QAction(MainWindow);
        show_same_action->setObjectName(QString::fromUtf8("show_same_action"));
        show_same_action->setCheckable(true);
        show_same_action->setEnabled(true);
        undo_action = new QAction(MainWindow);
        undo_action->setObjectName(QString::fromUtf8("undo_action"));
        undo_action->setEnabled(false);
        resume_aciton = new QAction(MainWindow);
        resume_aciton->setObjectName(QString::fromUtf8("resume_aciton"));
        resume_aciton->setEnabled(false);
        replace_action = new QAction(MainWindow);
        replace_action->setObjectName(QString::fromUtf8("replace_action"));
        show_source_action = new QAction(MainWindow);
        show_source_action->setObjectName(QString::fromUtf8("show_source_action"));
        show_source_action->setCheckable(true);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout_2 = new QHBoxLayout(centralwidget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(3, 3, 3, 3);
        open_file_tables = new QTabWidget(centralwidget);
        open_file_tables->setObjectName(QString::fromUtf8("open_file_tables"));
        open_file_tables->setTabsClosable(true);
        open_file_tables->setMovable(true);

        horizontalLayout_2->addWidget(open_file_tables);

        MainWindow->setCentralWidget(centralwidget);
        file_manage = new QDockWidget(MainWindow);
        file_manage->setObjectName(QString::fromUtf8("file_manage"));
        file_manage->setFeatures(QDockWidget::NoDockWidgetFeatures);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        horizontalLayout = new QHBoxLayout(dockWidgetContents);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        files_tree_manage = new FileTreeWidget(dockWidgetContents);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        files_tree_manage->setHeaderItem(__qtreewidgetitem);
        files_tree_manage->setObjectName(QString::fromUtf8("files_tree_manage"));
        files_tree_manage->header()->setVisible(false);

        horizontalLayout->addWidget(files_tree_manage);

        file_manage->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(Qt::LeftDockWidgetArea, file_manage);
        preview_manage = new QDockWidget(MainWindow);
        preview_manage->setObjectName(QString::fromUtf8("preview_manage"));
        preview_manage->setFeatures(QDockWidget::DockWidgetClosable);
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QString::fromUtf8("dockWidgetContents_2"));
        horizontalLayout_3 = new QHBoxLayout(dockWidgetContents_2);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        file_preview_text = new FileBrowser(dockWidgetContents_2);
        file_preview_text->setObjectName(QString::fromUtf8("file_preview_text"));
        file_preview_text->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        file_preview_text->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        file_preview_text->setLineWrapMode(QPlainTextEdit::NoWrap);
        file_preview_text->setReadOnly(true);

        horizontalLayout_3->addWidget(file_preview_text);

        preview_manage->setWidget(dockWidgetContents_2);
        MainWindow->addDockWidget(Qt::RightDockWidgetArea, preview_manage);
        status_bar = new QToolBar(MainWindow);
        status_bar->setObjectName(QString::fromUtf8("status_bar"));
        status_bar->setMinimumSize(QSize(0, 22));
        MainWindow->addToolBar(Qt::BottomToolBarArea, status_bar);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1440, 21));
        file_meun = new QMenu(menubar);
        file_meun->setObjectName(QString::fromUtf8("file_meun"));
        edit_menu = new QMenu(menubar);
        edit_menu->setObjectName(QString::fromUtf8("edit_menu"));
        MainWindow->setMenuBar(menubar);

        menubar->addAction(file_meun->menuAction());
        menubar->addAction(edit_menu->menuAction());
        file_meun->addAction(open_file_action);
        file_meun->addAction(open_json_action);
        file_meun->addAction(open_folder_action);
        file_meun->addSeparator();
        file_meun->addAction(export_checked_files_action);
        file_meun->addAction(export_all_files_action);
        file_meun->addSeparator();
        file_meun->addAction(export_checked_json_action);
        file_meun->addAction(export_now_json_action);
        file_meun->addAction(export_all_json_action);
        file_meun->addSeparator();
        edit_menu->addAction(undo_action);
        edit_menu->addAction(resume_aciton);
        edit_menu->addSeparator();
        edit_menu->addAction(show_preview_action);
        edit_menu->addAction(show_source_action);
        edit_menu->addAction(show_hideen_data_action);
        edit_menu->addAction(show_not_tran_action);
        edit_menu->addSeparator();
        edit_menu->addAction(replace_action);
        edit_menu->addSeparator();

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        open_file_action->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200\346\226\207\344\273\266", nullptr));
#if QT_CONFIG(shortcut)
        open_file_action->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        open_json_action->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200JSON", nullptr));
        open_folder_action->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200\346\226\207\344\273\266\345\244\271", nullptr));
#if QT_CONFIG(shortcut)
        open_folder_action->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Shift+O", nullptr));
#endif // QT_CONFIG(shortcut)
        export_checked_files_action->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\207\272\351\200\211\344\270\255\346\226\207\344\273\266", nullptr));
        export_all_files_action->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\207\272\345\205\250\351\203\250\346\226\207\344\273\266", nullptr));
        export_checked_json_action->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\207\272\351\200\211\344\270\255\347\232\204\350\257\215\346\261\207", nullptr));
        export_now_json_action->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\207\272\345\275\223\345\211\215\345\205\250\351\203\250\350\257\215\346\261\207", nullptr));
        export_all_json_action->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\207\272\346\225\260\346\215\256\345\272\223\345\205\250\351\203\250\350\257\215\346\261\207", nullptr));
        show_preview_action->setText(QCoreApplication::translate("MainWindow", "\346\230\276\347\244\272\351\242\204\350\247\210", nullptr));
        show_hideen_data_action->setText(QCoreApplication::translate("MainWindow", "\345\217\252\346\230\276\347\244\272\350\277\207\346\273\244\345\255\227\347\254\246\344\270\262", nullptr));
        show_not_tran_action->setText(QCoreApplication::translate("MainWindow", "\345\217\252\346\230\276\347\244\272\346\234\252\344\277\256\346\224\271\345\255\227\347\254\246\344\270\262", nullptr));
        show_same_action->setText(QCoreApplication::translate("MainWindow", "\345\217\252\346\230\276\347\244\272\351\207\215\345\244\215\345\255\227\347\254\246\344\270\262", nullptr));
        undo_action->setText(QCoreApplication::translate("MainWindow", "\346\222\244\345\233\236", nullptr));
#if QT_CONFIG(shortcut)
        undo_action->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Z", nullptr));
#endif // QT_CONFIG(shortcut)
        resume_aciton->setText(QCoreApplication::translate("MainWindow", "\346\201\242\345\244\215", nullptr));
#if QT_CONFIG(shortcut)
        resume_aciton->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Shift+Z", nullptr));
#endif // QT_CONFIG(shortcut)
        replace_action->setText(QCoreApplication::translate("MainWindow", "\346\211\271\351\207\217\346\233\277\346\215\242", nullptr));
#if QT_CONFIG(shortcut)
        replace_action->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+F", nullptr));
#endif // QT_CONFIG(shortcut)
        show_source_action->setText(QCoreApplication::translate("MainWindow", "\346\230\276\347\244\272\345\216\237\345\255\227\347\254\246\344\270\262", nullptr));
        file_manage->setWindowTitle(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266\350\265\204\346\272\220\347\256\241\347\220\206\345\231\250", nullptr));
        status_bar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
        file_meun->setTitle(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266", nullptr));
        edit_menu->setTitle(QCoreApplication::translate("MainWindow", "\347\274\226\350\276\221", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
