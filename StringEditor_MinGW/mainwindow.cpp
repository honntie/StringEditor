#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/Assets/icon.ico"));

    // 初始化
    ui->preview_manage->hide();

    // 底部状态
    ui->status_bar->layout()->setSpacing(10);
    ui->status_bar->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored);
    ui->status_bar->addWidget(log);

    // 读取上一次打开
    bool is_read = false;    // 是否读取成功
    QSqlQuery q = Sql::query("SELECT open_path FROM [OpenInfo] WHERE is_open = 1", false);
    while (q.next())
    {
        QFileInfo file_info(q.value(0).toString());
        if (!file_info.exists())
        {
            Sql::exec("DELETE FROM [OpenInfo] WHERE open_path = :1", {file_info.absoluteFilePath()});
            continue;
        }
        else is_read = true;

        ui->files_tree_manage->addRootPath(file_info.absoluteFilePath());
        showTreeItems();    //隐藏不存在字符串的item
    }
    if (is_read) log->setText("自动读取最后一次打开的路径! ");    //显示当前操作

    /////////////////////////
    /// 文件菜单 点击函数
    /////////////////////////

    //点击打开文件action
    connect(ui->open_file_action, &QAction::triggered, this, [=](){
        QString path = QFileDialog::getOpenFileName();
        if (ui->files_tree_manage->addRootPath(path) == false) return;
        openEvent(path);
    });

    //点击打开文件夹action
    connect(ui->open_folder_action, &QAction::triggered, this, [=](){
        QString path = QFileDialog::getExistingDirectory();
        if (ui->files_tree_manage->addRootPath(path) == false) return;
        openEvent(path);
    });

    //点击打开Json的action
    connect(ui->open_json_action, &QAction::triggered, this, [=](){
        QString path = QFileDialog::getOpenFileName(this,QString(), QString(), "Json(*.json);;(*.*)");
        if (path.isEmpty()) return;
        openJsonThread(path);
    });

    //点击导出选中文件action
    connect(ui->export_checked_files_action, &QAction::triggered, this, [=](){
        exportFileMode(true);    // 旧代码
    });

    //点击导出所有文件action
    connect(ui->export_all_files_action, &QAction::triggered, this, [=](){
        exportFileMode(false);
    });

    //点击导出选中文件的词汇
    connect(ui->export_checked_json_action, &QAction::triggered, this, [=](){
        QString export_folder = QFileDialog::getExistingDirectory();    //选择导出路径
        if (export_folder.isEmpty()) return;
        exportJsonThread(export_folder, ui->files_tree_manage->getShowPaths(true));
    });

    // 点击导出当前所有文件的词汇
    connect(ui->export_now_json_action, &QAction::triggered, this, [=](){
        QString export_folder = QFileDialog::getExistingDirectory();    //选择导出路径
        if (export_folder.isEmpty()) return;
        exportJsonThread(export_folder, ui->files_tree_manage->getShowPaths(false));
    });

    //点击导出所有词汇
    connect(ui->export_all_json_action, &QAction::triggered, this, [=](){
        QString export_folder = QFileDialog::getExistingDirectory();    //选择导出路径
        if (export_folder.isEmpty()) return;
        exportJsonThread(export_folder);
    });

    /////////////////////////
    /// 编辑菜单 点击函数
    /////////////////////////

    //点击撤回action
    connect(ui->undo_action, &QAction::triggered, this, [=](){
//        goHistory(PREVIOUS);
    });

    //点击恢复action
    connect(ui->resume_aciton, &QAction::triggered, this, [=](){
//        goHistory(NEXT);
    });

    //点击显示预览action
    connect(ui->show_preview_action, &QAction::triggered, this, [=](const bool &is_checed){
        if (is_checed) ui->preview_manage->show();
        else ui->preview_manage->hide();
    });

    //点击显示原字符串action
    connect(ui->show_source_action, &QAction::triggered, this, [=](){
        HSqlTableWidget *widget = static_cast<HSqlTableWidget*>(ui->open_file_tables->currentWidget());
        if (widget == nullptr) return;
        widget->setShowSourceData(ui->show_source_action->isChecked());
    });

    //点击只显示过滤数据action
    connect(ui->show_hideen_data_action, &QAction::triggered, this, [=](){
        showTreeItems();
        HSqlTableWidget *widget = static_cast<HSqlTableWidget*>(ui->open_file_tables->currentWidget());
        if (widget == nullptr) return;
        widget->show_hidden_data = ui->show_hideen_data_action->isChecked();
        widget->hideDataRow();
    });

    //点击只显示相同数据action
    connect(ui->show_same_action, &QAction::triggered, this, [=](){
        showTreeItems();
        HSqlTableWidget *widget = static_cast<HSqlTableWidget*>(ui->open_file_tables->currentWidget());
        if (widget == nullptr) return;
        widget->show_same_data = ui->show_same_action->isChecked();
        widget->hideDataRow();
    });

    //点击只显示未修改数据action
    connect(ui->show_not_tran_action, &QAction::triggered, this, [=](){
        showTreeItems();
        HSqlTableWidget *widget = static_cast<HSqlTableWidget*>(ui->open_file_tables->currentWidget());
        if (widget == nullptr) return;
        widget->show_not_changed_data = ui->show_not_tran_action->isChecked();
        widget->hideDataRow();
    });

    //点击批量替换action
    connect(ui->replace_action, SIGNAL(triggered()), replace_win, SLOT(show()));

    /////////////////////////
    /// FileTreeWidget控件函数
    /////////////////////////

    //资源管理器添加打开文件
    connect(ui->files_tree_manage, &FileTreeWidget::addDrogItem, this, &MainWindow::openEvent);

    //双击打开表格
    connect(ui->files_tree_manage, &FileTreeWidget::itemDoubleClicked, this, [=](QTreeWidgetItem *item){
        wait_time = QTime::currentTime();
        QString path = item->data(0, Qt::UserRole).toString();    //路径
        QString tab_title = item->data(0,Qt::DisplayRole).toString();    //选项卡名
        HSqlTableWidget *widget = ui->open_file_tables->findChild<HSqlTableWidget*>(path);

        //优化选项卡名
        if (item->child(0)) tab_title.remove(0, tab_title.lastIndexOf("/") + 1);

        if (widget == nullptr)
        {
            /* 新建widget */
            widget = new HSqlTableWidget(path, ui->open_file_tables);
            widget->setObjectName(path);
            updateTableState(widget);
            widget->createTable();

            /* 点击表格显示预览 */
            connect(widget, &HSqlTableWidget::tableClicked, this, [=](const QString &path,const int &index, QString string){
                if (ui->show_preview_action->isChecked() == false) return;
                ui->preview_manage->setWindowTitle(path);
                ui->file_preview_text->setHighlight(path, index, string);
                ui->preview_manage->show();
                ui->file_preview_text->setFocus();
            });

            /* 记录表格修改 */
            connect(widget, &HSqlTableWidget::tableChange, this, [=](const QString &source, const QString &replaced){
                QMap<QString, QString> action;
                action[source] = replaced;
                appendHistory(action);
            });
        }
        else if (ui->open_file_tables->indexOf(widget) == -1)
        {
            /* 表格已存在, 选项卡被删除, 手动刷新表格 */
            updateTableState(widget);
            widget->updateTable();
        }

        /* 加入QTabWidget */
        disconnect(ui->open_file_tables, &QTabWidget::currentChanged, this, &MainWindow::updateCurrentWidget);
        ui->open_file_tables->addTab(widget, item->icon(0), tab_title);
        ui->open_file_tables->setCurrentWidget(widget);
        connect(ui->open_file_tables, &QTabWidget::currentChanged, this, &MainWindow::updateCurrentWidget);

        /* 显示日志 */
        log->setText(QString("打开%1, 耗时%2ms").arg(path).arg(QString::number(wait_time.msecsTo(QTime::currentTime()))));
    });

    //资源管理器删除item
    connect(ui->files_tree_manage, &FileTreeWidget::removeFocusItem, this, [=](QString path){
        Sql::exec("UPDATE [OpenInfo] SET is_open = 0 WHERE open_path = :1", {path});
    });

    /////////////////////////
    /// 字符串表格函数
    /////////////////////////

    //点击点击表格关闭按钮函数
    connect(ui->open_file_tables, &QTabWidget::tabCloseRequested, this, [=](int index){
        ui->open_file_tables->removeTab(index);
        ui->preview_manage->setVisible(false);
    });

    //切换表格时更新表格
    connect(ui->open_file_tables, &QTabWidget::currentChanged, this, &MainWindow::updateCurrentWidget);

    /////////////////////////
    /// 替换窗口函数
    /////////////////////////

    //批量替换当前表格字符串
    connect(replace_win, &ReplaceWindow::repalceNowString, this, [=](const QString &source_str, const QString &replace_str){
        wait_time = QTime::currentTime();     //等待时间
        QMap<QString, QString> action = replaceCurrentWidgetString(source_str, replace_str);
        int count = replaceSqlString(action);    //替换数量 & 替换当前界面
        QMessageBox::information(replace_win, "替换当前",QString("一共替换%1处, 耗时%2秒")
                                     .arg(QString::number(count))
                                     .arg(QString::number(wait_time.msecsTo(QTime::currentTime()))),
                                 QMessageBox::Ok);
        appendHistory(action);
    });

    //批量替换全部字符串
    connect(replace_win, &ReplaceWindow::repalceAllString, this, [=](const QString &source, const QString &replace){
        wait_time = QTime::currentTime();    //等待时间
        QMap<QString, QString> str_dict = replaceCurrentWidgetString(source, replace);    //需要替换的字符串内容(包含当前界面的)

        /* 追加需要替换的字符串 */
        QString source_str;     //需要替换的字符串
        QString replace_str;     //被替换的字符串

        QSqlQuery q = Sql::query(QString("SELECT cook_text FROM [StrInfo] WHERE cook_text LIKE '%%1%'").arg(source), false);
        while (q.next())
        {
            source_str = replace_str = q.value(0).toString();
            replace_str.replace(source, replace);
            str_dict[source_str] = replace_str;
        }
        //appendReplaceString(str_dict, source, replace);    // 追加需要替换的字符串

        /* 开始替换字符串 */
        int count = replaceSqlString(str_dict);    //替换数量
        QMessageBox::information(replace_win, "替换全部",
                                 QString("一共替换%1处, 耗时%2秒")
                                     .arg(QString::number(count))
                                     .arg(QString::number(wait_time.msecsTo(QTime::currentTime()))),
                                  QMessageBox::Ok);
        appendHistory(str_dict);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openEvent(const QString &path)
{
    QFileInfo info(path);

    /* 读取Json词汇 */
    if (info.completeSuffix().contains("json", Qt::CaseInsensitive)) openJsonThread(path);
    else
    {
        QDateTime lase_modified = info.lastModified();
        if (Sql::isExist("SELECT * FROM [OpenInfo] WHERE open_path = :1", {path}) == false)
        {
            /* 不存在，加入新数据 */
            Sql::exec("INSERT INTO [OpenInfo] VALUES(:1, 1, :2)",{path, lase_modified});
            openFileThread(path);

            /* 数据库中已存在改数据情况(不合理情况) */
    //        int i = 0;
    //        if (Sql::isExist(QString("SELECT * FROM [FileInfo] Where file_path LIKE '%1%'").arg(path)))
    //            i = QMessageBox::information(this, "提示", path + "\n该路径已经被读取过一次，是否重新读取字符串" ,"是", "否");
    //        if (i == 0) openFileThread(path);
        }
        else if (QMessageBox::information(this, "提示", path + "\n该路径已存在，是否覆盖旧数据" ,"是", "否") == 0)
        {
            /* 存在, 覆盖旧数据 */
            Sql::exec(QString("DELETE FROM [FileInfo] WHERE file_path LIKE '%1%'").arg(path));
            openFileThread(path);
        }
        else
        {
            /* 存在, 不覆盖旧数据 */
            log->setText("路径在数据库已存在，无需读取！");    //显示当前操作
            showTreeItems();    //隐藏不存在字符串的item
        }
        Sql::exec("UPDATE [OpenInfo] SET is_open = 1 WHERE open_path = :1", {path});    //记录路径
    }
}

void MainWindow::updateCurrentWidget(const int &index)
{
    if (index == -1) return;
    wait_time = QTime::currentTime();
    HSqlTableWidget *w = static_cast<HSqlTableWidget*>(ui->open_file_tables->widget(index));
    updateTableState(w);
    w->updateTable();
    log->setText(QString("切换%1, 消耗时间%2ms").arg(w->getPath()).arg(QString::number(wait_time.msecsTo(QTime::currentTime()))));
}

void MainWindow::updateWidget(int index)
{
    if (index == -1) return;
    HSqlTableWidget *w = static_cast<HSqlTableWidget*>(ui->open_file_tables->widget(index));
    updateTableState(w);
    w->updateTable();
}

void MainWindow::showTreeItems()
{
    QStringList hidden_paths;
    QStringList shown_paths;
    QString path;
    QString source_text;
    QString replace_text;
//    bool is_shown;
    QTreeWidgetItem *item;
    for (int i = 0; i < ui->files_tree_manage->topLevelItemCount(); i++)
    {
        item = ui->files_tree_manage->topLevelItem(i);
        QSqlQuery q = Sql::query(QString("SELECT [FileInfo].file_path, [FileInfo].str_text, [StrInfo].cook_text FROM [FileInfo] "
                                         "INNER JOIN [StrInfo] ON [FileInfo].str_text = [StrInfo].raw_text "
                                         "WHERE [FIleInfo].file_path LIKE '%1%'").arg(item->data(0, Qt::UserRole).toString()), false);
        shown_paths.clear();
        hidden_paths.clear();
        while (q.next())
        {
//            is_shown = false;
            path = q.value(0).toString();
            source_text = q.value(1).toString();
            replace_text = q.value(2).toString();
            if (shown_paths.contains(path)) continue;
            else if (ui->show_hideen_data_action->isChecked() == source_text.contains(QRegExp("[^\u0020-\u007E]+"))) continue;
            else if (ui->show_not_tran_action->isChecked() && source_text == replace_text) continue;
            shown_paths.append(path);
        }
        ui->files_tree_manage->setShownItem(item, shown_paths);
    }
    return;

    QStringList front_paths;    //绝对路径组
    QString front_path;    //绝对路径
    QList<QStringList> last_paths;    //相对路径
    for (int i = 0; i < ui->files_tree_manage->topLevelItemCount(); i++)
    {
        front_path = ui->files_tree_manage->topLevelItem(i)->data(0, 0).toString();
        front_paths.append(front_path);
        QStringList last_path;
        QSqlQuery q = Sql::query(QString("SELECT SUBSTR([FileInfo].file_path, length('%1')+2) FROM [FileInfo] "
                                         "INNER JOIN [StrInfo] ON [FileInfo].str_text = [StrInfo].raw_text "
                                         "WHERE [FileInfo].file_path LIKE '%1%' AND [FileInfo].str_text %3 REGEXP %2 %4 %5 "
                                         "GROUP BY [FileInfo].file_path")
                                 .arg(front_path).arg("'[^\u0020-\u007E]+'")
                                 .arg(ui->show_hideen_data_action->isChecked() ? "NOT":"")
                                 .arg(ui->show_same_action->isChecked() ? QString("AND [FileInfo].str_text IN (SELECT str_text FROM [FileInfo] WHERE file_path LIKE '%1%' GROUP BY str_text HAVING count(*) >= 2) ").arg(front_path):"")
                                 .arg(ui->show_not_tran_action->isChecked() ? "AND [FileInfo].str_text = [StrInfo].cook_text " : ""), false);
        while (q.next()) last_path.append(q.value(0).toString());
        last_paths.append(last_path);
    }
    ui->files_tree_manage->showPathItem(front_paths, last_paths);
}

int MainWindow::replaceSqlString(const QMap<QString, QString> &dict)
{
    int button;    //点击按钮
    int count = 0;    //替换数量

    bool is_trannsaction = Sql::transaction();    //开始事务
    for (QMap<QString, QString>::const_iterator iter = dict.begin(); iter != dict.end(); iter++)
    {
        Sql::exec("UPDATE [StrInfo] SET cook_text = :2 WHERE cook_text = :1", {iter.key(), iter.value()});
        count++;
    }

    //结束事务
    bool is_finish = Sql::commit();
    while (is_trannsaction != is_finish)
    {
        button = QMessageBox::warning(this, "替换当前",
                                      QString("数据库事务状态异常\n开启事务:%1 \n 提交事务: %2")
                                      .arg(QString::number(is_trannsaction))
                                      .arg(QString::number(is_finish)),
                                      "重试", "取消");
        if (button == 1) break;
        is_finish = QSqlDatabase::database().commit();
    }
    return count;
}

QMap<QString, QString> MainWindow::replaceCurrentWidgetString(const QString &source_str, const QString &replace_str)
{
    HSqlTableWidget *widget = static_cast<HSqlTableWidget*>(ui->open_file_tables->currentWidget());
    if (widget == nullptr || source_str.isEmpty()) return QMap<QString, QString>();
    return widget->replaceStringData(source_str, replace_str);
}

void MainWindow::appendReplaceString(QMap<QString, QString> &str_dict, const QString &source, const QString &replace) const
{
    QString source_str;     //需要替换的字符串
    QString replace_str;     //被替换的字符串

    /* 寻找需要替换的字符串 */
    QSqlQuery q = Sql::query(QString("SELECT cook_text FROM [StrInfo] WHERE cook_text LIKE '%%1%'").arg(source), false);
    while (q.next())
    {
        source_str = replace_str = q.value(0).toString();
        replace_str.replace(source, replace);
        str_dict[source_str] = replace_str;
    }
}

void MainWindow::appendHistory(const QMap<QString, QString> &action)
{
    if (now_action.isEmpty() == false) history.append(now_action);
    now_action = action;
}

void MainWindow::exportFileMode(bool is_checked)
{
    QString export_folder = QFileDialog::getExistingDirectory();    //选择导出路径
    if (export_folder.isEmpty()) return;

    int top_item_count = ui->files_tree_manage->topLevelItemCount();
    if (top_item_count == 0) return;

    QMap<QString, QString> paths;     // key: 需要导出的文件, value: 导出的位置
    QTreeWidgetItem *top_item = ui->files_tree_manage->topLevelItem(0);     // 顶层item
    QString export_path;    // 导出的文件路径
    QString root_path;    // 顶层item的父路径

    if (top_item_count == 1 && top_item->childCount() == 0)
    {
        if (!top_item->data(0, Qt::CheckStateRole).toBool() && is_checked) return;
        root_path = export_path = top_item->data(0, Qt::UserRole).toString();
        export_path.replace(root_path.left(root_path.lastIndexOf('/')), export_folder);
        paths[root_path] = export_path;
    }
    else for (int i = 0; i < top_item_count; i++)
    {
        top_item = ui->files_tree_manage->topLevelItem(i);
        root_path = top_item->data(0, Qt::UserRole).toString();
        root_path = root_path.left(root_path.lastIndexOf('/'));
        QStringList export_files = ui->files_tree_manage->getShowPaths(top_item, is_checked);
        for (const QString &file_path : export_files)
        {
            export_path = file_path;
            export_path.replace(root_path, export_folder);
            paths[file_path] = export_path;
        }
    }

    if (paths.isEmpty()) return;
    exportFileThread(paths);    //开启线程
}

void MainWindow::openFileThread(const QString &path)
{
    ExtractStringThread *thread = new ExtractStringThread(path);
    readyThread(thread, [=](){
        log->setText(QString("提取完成，共耗时%1秒").arg(QString::number(wait_time.msecsTo(QTime::currentTime()) / 1000.0, 'f', 2)));
    });
    thread->start();
}

void MainWindow::openJsonThread(const QString &path)
{
    ImportStringThread *thread = new ImportStringThread(path, this);
    readyThread(thread, [=](){
        updateWidget(ui->open_file_tables->currentIndex());
        log->setText(QString("导出完成, 共加入或更新%1个词汇, 共耗时%2秒")
                            .arg(QString::number(thread->GetStringCount()))
                            .arg(QString::number(wait_time.msecsTo(QTime::currentTime()) / 1000.0, 'f', 2)));
    });
    thread->start();
}

void MainWindow::exportFileThread(const QMap<QString, QString> &path_map)
{
    ExportFileThread *thread = new ExportFileThread(path_map);
    readyThread(thread, [=](){
        log->setText(QString("导出完成，共耗时%1秒").arg(QString::number(wait_time.msecsTo(QTime::currentTime()) / 1000.0, 'f', 2)));
    });
    thread->start();
}

void MainWindow::exportJsonThread(const QString &export_path, const QStringList &file_paths)
{
    QTimer *timer = new QTimer(this);
    timer->start(0.5);
    connect(timer, &QTimer::timeout, this, [=](){
        QString text = log->text();
        int dot = text.count('.');
        if (dot == 9) log->setText("正在导出中...");
        else log->setText(text + "...");
    });

    ExportStringThread *thread = new ExportStringThread(export_path, file_paths, ui->show_hideen_data_action->isChecked());
    readyThread(thread, [=]() {
        timer->stop();
        delete timer;
        log->setText(QString("导出完成，共耗时%1秒").arg(QString::number(wait_time.msecsTo(QTime::currentTime()) / 1000.0, 'f', 2)));
    });
    thread->start();
}

void MainWindow::updateTableState(HSqlTableWidget *widget)
{
    widget->setShowSourceData(ui->show_source_action->isChecked());
    widget->show_same_data = ui->show_same_action->isChecked();
    widget->show_not_changed_data = ui->show_not_tran_action->isChecked();
    widget->show_hidden_data = ui->show_hideen_data_action->isChecked();
}
