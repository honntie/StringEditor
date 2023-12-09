    #include "hsqltablewidget.h"

HSqlTableWidget::HSqlTableWidget(const QString &file_path, QWidget *parent) :
    QWidget(parent), path(file_path)
{
    /* 设置搜索框 */
    search_edit->setMaximumHeight(30);
    search_edit->setPlaceholderText("搜索字符串");
    layout->addWidget(search_edit, 0);

    /* 设置表 */
    view->setColumnCount(4);
    view->setHorizontalHeaderLabels({"文件", "位置", "字符串", "原字符串"});
    view->setSortingEnabled(false);
//    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    this->installEventFilter(this);

    /* 设置布局 */
    layout->setSizeConstraint(QLayout::SetMaximumSize);
    layout->setStretch(0, 1);
    layout->setStretch(1, 9);
    layout->setMargin(0);
    layout->setSpacing(0);
    this->setLayout(layout);
    layout->addWidget(search_edit, 0);
    layout->addWidget(view, 1);

    /* 槽函数连接 */
    //点击排序
    connect(view->horizontalHeader(), &QHeaderView::sectionClicked, this, [=](const int &colum){
        if (sort_num[0] == colum) is_desc = (is_desc == false);
        else
        {
            is_desc = false;
            int num = colum;
            for (int i = 0; i < 4; i++)
            {
                int next = sort_num[i];
                sort_num[i] = num;
                if (next == colum) break;
                num = next;
            }
        }
        sortData();
        hideDataRow();
    });

    //点击表格
    connect(view, &QTableWidget::clicked, this, [=](const QModelIndex &model){
        QString path = model.sibling(model.row(), 0).data(Qt::UserRole).toString();
        int index = model.sibling(model.row(), 1).data().toInt();
        QString str = model.sibling(model.row(), 3).data().toString();
        emit tableClicked(path, index, str);
    });

    //输入搜索字符串
    connect(search_edit, &QLineEdit::textChanged, this, [=](){
        disconnect(view->verticalScrollBar(), &QScrollBar::valueChanged, this, HSqlTableWidget::updateCurrentItems);
        disconnect(view->verticalScrollBar(), &QScrollBar::rangeChanged, this, HSqlTableWidget::updateCurrentItems);

        hideDataRow();

        connect(view->verticalScrollBar(), &QScrollBar::valueChanged, this, HSqlTableWidget::updateCurrentItems);
        connect(view->verticalScrollBar(), &QScrollBar::rangeChanged, this, HSqlTableWidget::updateCurrentItems);
    });
}

HSqlTableWidget::~HSqlTableWidget()
{
    delete search_edit;
    delete layout;
    delete view;
    delete[] load_data;
}

QString HSqlTableWidget::getDataQuery() const
{
    return QString("SELECT [FileInfo].file_path, [FileInfo].str_index, [StrInfo].cook_text, [FileInfo].str_text FROM [FileInfo] "
                   "INNER JOIN [StrInfo] ON [FileInfo].str_text = [StrInfo].raw_text "
                   "WHERE [FileInfo].file_path LIKE '%1%' "
                   "GROUP BY [FileInfo].str_text").arg(path);
}

QString HSqlTableWidget::getSizeQuery() const
{
    return QString("SELECT COUNT(*) FROM [FileInfo] "
                   "INNER JOIN [StrInfo] ON [FileInfo].str_text = [StrInfo].raw_text "
                   "WHERE [FileInfo].file_path LIKE '%1%' "
                   "GROUP BY [FileInfo].str_text").arg(path);
}

void HSqlTableWidget::hideDataRow()
{
    for (int i = 0; i < data_size; i++)
        view->setRowHidden(i, isHiddenData(load_data[i]));
    updateCurrentItems();
}

void HSqlTableWidget::createLoadData()
{
    if (load_data) delete[] load_data;
    QSqlQuery q = Sql::query(getDataQuery(), false);
    data_size = 0;
    while (q.next()) data_size++;
    if (data_size <= 0) return;

    load_data = new TableData[data_size];
    q.seek(-1);
    while (q.next()) load_data[q.at()] = getSqlData(q);
    sortData();    //排序
}

int HSqlTableWidget::compareData(const TableData &data1, const TableData &data2)
{
    int result;
    for (short i : sort_num)
    {
        switch (i)
        {
            // 路径
            case 0: result = QString::compare(data1.path, data2.path); break;
            // 位置
            case 1: result = (int)(data1.index - data2.index); break;
            // 修改字符串
            case 2: result = QString::compare(data1.replace_str, data2.replace_str); break;
            // 原字符串
            case 3: result = QString::compare(data1.source_str, data2.source_str); break;
        }
        if (result == 0) continue;
        return result;
    }
    return 0;
}

void HSqlTableWidget::quickSort(TableData *array, int length, int begin)
{
    if (length < 2) return;
    else if (length == 2)
    {
        if ((compareData(array[begin], array[begin + 1]) < 0) == is_desc)
            std::swap(array[begin], array[begin + 1]);
        return;
    }
    int left = begin;
    int right = begin + length;
    TableData spilt = array[begin + length / 2];
    bool is_right = true;

    std::swap(array[begin], array[begin + length / 2]);

    while (left != right)
    {
        if (is_right)
        {
            right--;
            if ((compareData(array[right], spilt) > 0) == is_desc)
            {
                array[left] = array[right];
                is_right = false;
            }
        }
        else
        {
            left++;
            if ((compareData(array[left], spilt) < 0) == is_desc)
            {
                array[right] = array[left];
                is_right = true;
            }
        }
    }
    array[left] = spilt;
    quickSort(array, left - begin, begin);
    quickSort(array, begin + length - right - 1, right + 1);
}

void HSqlTableWidget::sortData()
{
    quickSort(load_data, data_size);
    for (int i = 0; i < data_size; i++) load_data[i].is_updated = false;
}

void HSqlTableWidget::iniRowItems(const int &row)
{
    view->setItem(row, 0, new QTableWidgetItem());
    view->setItem(row, 1, new QTableWidgetItem());
    view->setItem(row, 2, new QTableWidgetItem());
    view->setItem(row, 3, new QTableWidgetItem());
}

bool HSqlTableWidget::isHiddenData(const TableData &data) const
{
    // 是否隐藏过滤数据
    if (data.source_str.contains(QRegExp("[^\u0020-\u007E]+")) == show_hidden_data) return true;
    // 是否隐藏已修改数据
    else if (data.replace_str != data.source_str && show_not_changed_data) return true;
    // 搜索框是否有文本
    else if (search_edit->text().isEmpty() == false)
    {
        QString fifter = search_edit->text();
        if (data.path.contains(fifter)) return false;
        else if (QString::number(data.index).contains(fifter)) return false;
        else if (data.replace_str.contains(fifter)) return false;
        else if (data.source_str.contains(fifter)) return false;
        return true;
    }
    return false;
}

void HSqlTableWidget::appendRowData(const TableData &data)
{
    //文件路径item
    QTableWidgetItem *path_item = new QTableWidgetItem();
    QString long_path = data.path;
    QString short_path = long_path == path ? path.mid(path.lastIndexOf('/') + 1, path.length() - 1):QString(long_path).remove(path + "/");
    path_item->setData(Qt::UserRole, long_path);
    path_item->setData(Qt::DisplayRole, short_path);
    path_item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

    //字符串位置item
    QTableWidgetItem *index_item = new QTableWidgetItem();
    index_item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    index_item->setData(Qt::DisplayRole, data.index);

    //字符串item
    QTableWidgetItem *str_item = new QTableWidgetItem(data.replace_str);

    //原字符串item
    QTableWidgetItem *raw_item = new QTableWidgetItem(data.source_str);

    //加入数据
    view->setRowCount(view->rowCount() + 1);
    view->setItem(view->rowCount() - 1, 3, raw_item);
    view->setItem(view->rowCount() - 1, 2, str_item);
    view->setItem(view->rowCount() - 1, 1, index_item);
    view->setItem(view->rowCount() - 1, 0, path_item);
}

void HSqlTableWidget::updateRowData(const int &row)
{
    if (row < 0 || row >= view->rowCount()) return;
    else if (load_data[row].is_updated) return;
    TableData *data = &load_data[row];
    data->is_updated = true;

    //文件路径item
    QTableWidgetItem *path_item = view->item(row, 0);
    QString long_path = data->path;
    QString short_path = long_path == path ? path.mid(path.lastIndexOf('/') + 1, path.length() - 1):QString(long_path).remove(path + "/");
    path_item->setData(Qt::UserRole, long_path);
    path_item->setData(Qt::DisplayRole, short_path);
    path_item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

    //字符串位置item
    QTableWidgetItem *index_item = view->item(row, 1);
    index_item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    index_item->setData(Qt::DisplayRole, data->index);

    //字符串item
    QTableWidgetItem *str_item = view->item(row, 2);
    str_item->setData(Qt::DisplayRole, data->replace_str);

    //原字符串item
    QTableWidgetItem *raw_item = view->item(row, 3);
    raw_item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    raw_item->setData(Qt::DisplayRole, data->source_str);
}

TableData HSqlTableWidget::getSqlData(const QSqlQuery &query) const
{
    TableData data;
    data.path = query.value(0).toString();
    data.index = query.value(1).toInt();
    data.replace_str = query.value(2).toString();
    data.source_str = query.value(3).toString();
    data.is_updated = false;
    return data;
}

void HSqlTableWidget::sqlUpdate(const QTableWidgetItem *item)
{
    QString changed = view->item(item->row(), 2)->data(Qt::DisplayRole).toString();    //修改字符串
    QString source = view->item(item->row(), 3)->data(Qt::DisplayRole).toString();    //原字符串
    load_data[item->row()].replace_str = changed;
    Sql::exec("UPDATE [StrInfo] SET cook_text = :1 WHERE raw_text = :2", {changed, source});
    emit tableChange(source, changed);
}

short HSqlTableWidget::compareString(const QString &str1, const QString &str2)
{
    return QString::compare(str1, str2);
    int min_length = str1.length() < str2.length() ? str1.length():str2.length();
//    short compare;
    for (int i = 0; i < min_length; i++)
    {
        if (str1.at(i) == str2.at(i)) return 0;
        else return str1.at(i) > str2.at(i) ? -1:1;
    }
    if (str1.length() == str2.length()) return 0;
    else if (str1.length() > str2.length()) return -1;
    else return 1;
}

void HSqlTableWidget::setShowSourceData(const bool &is_show_source_data)
{
    view->setColumnHidden(3, is_show_source_data == false);
}

/* 遗弃代码
void HSqlTableWidget::setShowHideData(const bool &is_show_hide_data)
{
    show_hidden_data = is_show_hide_data;
}

void HSqlTableWidget::setShowSame(const bool &is_show_same)
{
     show_same_data = is_show_same;
}

void HSqlTableWidget::setShowNotTran(const bool &is_show_not_translate)
{
    show_not_changed_data = is_show_not_translate;
}
*/

void HSqlTableWidget::createTable()
{
    createLoadData();    // 初始化数据

    /* 初始化行数 */
    view->setRowCount(data_size);
    for (int i = 0; i < data_size; i++) iniRowItems(i);
    hideDataRow();

    /* 修改更新表 */
    connect(view, &QTableWidget::itemChanged, this, &HSqlTableWidget::sqlUpdate);

    /* 加载数据 */
    connect(view->verticalScrollBar(), &QScrollBar::valueChanged, this, HSqlTableWidget::updateCurrentItems);
    connect(view->verticalScrollBar(), &QScrollBar::rangeChanged, this, HSqlTableWidget::updateCurrentItems);
}

void HSqlTableWidget::updateCurrentItems()
{
    QTableWidgetItem *item = view->itemAt(view->columnAt(0), 0);
    if (item == nullptr) return;
    disconnect(view, &QTableWidget::itemChanged, this, &HSqlTableWidget::sqlUpdate);

    int item_height = view->rowHeight(item->row());     //行高
    updateRowData(item->row());    //更新第一个item
    for (int i = item_height; i < height() - search_edit->height(); i += item_height)
    {
        updateRowData(item->row());
        item = view->itemAt(view->columnAt(0), i);
        if (item == nullptr) break;
    }

    connect(view, &QTableWidget::itemChanged, this, &HSqlTableWidget::sqlUpdate);
}

void HSqlTableWidget::updateTable()
{
    /* 初始化数据 */
    createLoadData();

    disconnect(view->verticalScrollBar(), &QScrollBar::valueChanged, this, HSqlTableWidget::updateCurrentItems);
    disconnect(view->verticalScrollBar(), &QScrollBar::rangeChanged, this, HSqlTableWidget::updateCurrentItems);
    disconnect(view, &QTableWidget::itemChanged, this, &HSqlTableWidget::sqlUpdate);

    int view_row = view->rowCount();
    /* 为添加数据扩充空间 */
    if (view_row < data_size)
    {
        for (; view_row < data_size; view_row++)
        {
            view->insertRow(view_row);
            iniRowItems(view_row);
        }
    }
    /* 删除多余数据 */
    else for (; view_row >= data_size; --view_row)
            view->removeRow(view_row);
    hideDataRow();    // 隐藏过滤数据

    connect(view, &QTableWidget::itemChanged, this, &HSqlTableWidget::sqlUpdate);
    connect(view->verticalScrollBar(), &QScrollBar::valueChanged, this, HSqlTableWidget::updateCurrentItems);
    connect(view->verticalScrollBar(), &QScrollBar::rangeChanged, this, HSqlTableWidget::updateCurrentItems);
}

QString HSqlTableWidget::getPath() const
{
    return path;
}

QMap<QString, QString> HSqlTableWidget::replaceStringData(const QString &source, const QString &replace)
{
    QMap<QString, QString> result;
    TableData *data;
    for (int i = 0; i < data_size; i++)
    {
        data = &load_data[i];
        if (data->replace_str.contains(source))
        {
            data->is_updated = false;
            QString str = data->replace_str;
            result[str] = data->replace_str.replace(source, replace);
        }
    }
    updateCurrentItems();
    return result;
}

void HSqlTableWidget::changeStringData(const QString &source, const QString &changed)
{
    for (int i = 0; i < data_size; i++)
    {
        if (load_data[i].replace_str == source)
        {
            load_data[i].is_updated = false;
            load_data[i].replace_str = changed;
        }
    }
    updateCurrentItems();
}

void HSqlTableWidget::keyPressEvent(QKeyEvent *et)
{
    if (et->key() == Qt::Key_Control) is_ctrl = true;
}

void HSqlTableWidget::keyReleaseEvent(QKeyEvent *et)
{
    if (et->key() == Qt::Key_Control) is_ctrl = false;
}
