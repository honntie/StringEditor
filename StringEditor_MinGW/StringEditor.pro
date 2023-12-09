QT       += core gui sql
VERSION = 0.20.220611
RC_FILE += icon.rc

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    # 控件文件
    Component/filebrowser.cpp \
    Component/filetreewidget.cpp \
    Component/hsqltablewidget.cpp \
    # 工具方法文件
    Handle/sql.cpp \
    Handle/iniseting.cpp \
    # 线程类文件
    Thread/exportfilethread.cpp \
    Thread/exportstringthread.cpp \
    Thread/extractstringthread.cpp \
    Thread/filecontroltread.cpp \
    # 线程池文件
    Thread/Runnable/hrunnable.cpp \
    Thread/Runnable/exportfilerunnable.cpp \
    Thread/Runnable/extractstringrunnable.cpp \
    # 窗体和其他文件
    Thread/importstringthread.cpp \
    Thread/progressthread.cpp \
    main.cpp \
    mainwindow.cpp \
    replacewindow.cpp

HEADERS += \
    #控件文件
    Component/filebrowser.h \
    Component/filetreewidget.h \
    Component/hsqltablewidget.h \
    #工具方法文件
    Handle/definitionvalue.h \
    Handle/iniseting.h \
    Handle/sql.h \
    Handle/historyaction.h \
    #线程类文件
    Thread/exportfilethread.h \
    Thread/exportstringthread.h \
    Thread/extractstringthread.h \
    Thread/filecontroltread.h \
    # 线程池文件
    Thread/Runnable/exportfilerunnable.h \
    Thread/Runnable/extractstringrunnable.h \
    Thread/Runnable/hrunnable.h \
    #窗体和其他文件
    Thread/importstringthread.h \
    Thread/progressthread.h \
    mainwindow.h \
    replacewindow.h

FORMS += \
    mainwindow.ui \
    replacewindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    icon.rc

RESOURCES += \
    assets.qrc
