#include "filetreewidget.h"

FileTreeWidget::FileTreeWidget(QWidget *parent) : QTreeWidget(parent)
{
    // 设置接受拖动文件
    setDragDropMode(QAbstractItemView::InternalMove);
    setAcceptDrops(true);

    // TreeWidget右键菜单
    menu->addAction(remove_action);
    setContextMenuPolicy(Qt::CustomContextMenu);

    //右键弹出菜单
    connect(this, &FileTreeWidget::customContextMenuRequested, this, [=](){
        focus_items = this->selectedItems();
        if (focus_items.isEmpty()) return;
        menu->move(this->cursor().pos());
        menu->show();
    });

    //点击 "删除打开文件" action
    connect(remove_action, &QAction::triggered, this, [=](){
        QTreeWidgetItem *item = this->selectedItems().first();
        if (item == nullptr) return;
        QTreeWidgetItem *topItem = this->getTopItem(item);
        emit removeFocusItem(topItem->data(0,0).toString());
        removeItem(topItem);
    });
}

bool FileTreeWidget::isPathExist(const QString &path) const
{
    for (const QString &root_path : root_path_items.keys())
    {
        QDir dir(root_path);
        QStringList folder_paths = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        QStringList file_paths = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
        QString folder_path;    //文件夹路径
        QString file_path;    //文件路径

        // 获取文件夹下文件
        for (const QString &short_path : folder_paths)
        {
            folder_path = path + "/" + short_path;
            if (isPathExist(folder_path)) return true;
        }

        // 获取文件
        for (const QString &short_path : file_paths)
        {
            file_path = path + "/" + short_path;
            if (file_path == path) return true;
        }
    }
    return false;
}

QTreeWidgetItem *FileTreeWidget::createFileItem(const QString &path , QString name)
{
    name = name.isEmpty() ? path.right(path.length() - 1 - path.lastIndexOf("/")) : name;

    // 获取文件图标
    QFileIconProvider icon_provider;
    QIcon icon = icon_provider.icon(QFileInfo(path));

    // 初始化item
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setIcon(0,icon);
    item->setText(0,name);
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
    item->setCheckState(0,Qt::Checked);
    item->setData(0,Qt::UserRole,QVariant(path));

    all_path_items[path] = item;
    return item;
}

QList<QTreeWidgetItem*> FileTreeWidget::createFolderItems(const QString &path)
{
    QList<QTreeWidgetItem*> all_item;

    QDir dir(path);
    QStringList folder_paths = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList file_paths = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
    QString folder_path;    //文件夹路径
    QString file_path;    //文件路径

    // 获取文件夹下文件
    for (const QString &short_path : folder_paths)
    {
        folder_path = path + "/" + short_path;
        QTreeWidgetItem *folder_item = createFileItem(folder_path);
        folder_item->addChildren(createFolderItems(folder_path));
        all_item.append(folder_item);
    }

    // 获取文件
    for (const QString &short_path : file_paths)
    {
        file_path = path + "/" + short_path;
        all_item.append(createFileItem(file_path));
    }

    return all_item;
}

QList<QTreeWidgetItem *> FileTreeWidget::getItemChildren(QTreeWidgetItem *item) const
{
    QList<QTreeWidgetItem *> children;
    for (int i = 0; i < item->childCount() && item; i++)
    {
        QTreeWidgetItem *child = item->child(i);
        if (child->childCount() > 0) children.append(getItemChildren(child));
        children.append(child);
    }
    return children;
}

QTreeWidgetItem *FileTreeWidget::getTopItem(QTreeWidgetItem *item) const
{
    QTreeWidgetItem *topItem = item;
    while (topItem->parent() != nullptr)
        topItem = topItem->parent();
    return topItem;
}

bool FileTreeWidget::addRootPath(const QString &root_path)
{
    if (QFileInfo::exists(root_path) == false) return false;
    if (all_path_items.contains(root_path) || root_path_items.contains(root_path)) return false;

    QTreeWidgetItem *topItem = createFileItem(root_path, root_path);
    QList<QTreeWidgetItem*> items = createFolderItems(root_path);
    topItem->addChildren(items);
    topItem->sortChildren(0, Qt::AscendingOrder);
    this->addTopLevelItem(topItem);

    root_path_items[root_path] = topItem;
    all_path_items.remove(root_path);

    return true;
}

QStringList FileTreeWidget::getShowPaths(const bool &filter_checked)
{
    QStringList result;
    for (int i = 0;i < topLevelItemCount(); i++)
        result.append(getShowPaths(topLevelItem(i), filter_checked));
    return result;
}

QStringList FileTreeWidget::getShowPaths(QTreeWidgetItem *topItem, const bool &filter_checked)
{
    // 判断筛选条件
    if ((topItem->data(0,Qt::CheckStateRole).toBool() == false && filter_checked) || topItem->isHidden())
        return QStringList();

    // 没有子item
    else if (topItem->childCount() == 0)
        return QStringList(topItem->data(0,Qt::UserRole).toString());

    // 有子item
    QStringList result_paths;
    for (int index = 0; index < topItem->childCount(); index++)
        result_paths.append(getShowPaths(topItem->child(index), filter_checked));

    return result_paths;
}

void FileTreeWidget::showPathItem(const QStringList &front_paths, const QList<QStringList> &last_paths)
{
    QStringList hidden_paths = all_path_items.keys();
    QString show_path;    //显示路径
    // 筛选隐藏的item路径
    for (const QString &path : all_path_items.keys())
    {
        QTreeWidgetItem *parent = all_path_items[path]->parent();
        this->setRowHidden(this->indexFromItem(all_path_items[path]).row(), this->indexFromItem(parent), false);
    }

    // 分割并筛选路径
    for (const QString &front_path : front_paths)
    {
        for (const QStringList &path_list : last_paths)
        {
            for (const QString &path : path_list)
            {
                show_path = front_path;
                for (const QString &str : path.split("/"))
                {
                    show_path += "/" + str;
                    hidden_paths.removeOne(show_path);
                }
            }
        }
    }

    // 开始隐藏
    for (QString path : hidden_paths)
    {
        if (all_path_items.contains(path))
        {
            QTreeWidgetItem *parent = all_path_items[path]->parent();
            this->setRowHidden(this->indexFromItem(all_path_items[path]).row(), this->indexFromItem(parent), true);
        }
    }
}

bool FileTreeWidget::setShownItem(QTreeWidgetItem *item, const QStringList &shown_paths)
{
    bool is_shown = false;
    if (item->childCount() <= 0) is_shown = shown_paths.contains(item->data(0, Qt::UserRole).toString());
    else for (int i = 0; i < item->childCount(); i++)
    {
        if (setShownItem(item->child(i), shown_paths))
            is_shown = true;
    }
    setRowHidden(this->indexFromItem(item).row(), this->indexFromItem(item->parent()), is_shown == false);
    return is_shown;
}

void FileTreeWidget::removeItem(QTreeWidgetItem *item)
{
    if (item == nullptr) return;
    root_path_items.remove(item->data(0, Qt::UserRole).toString());
    all_path_items.remove(item->data(0, Qt::UserRole).toString());
    for (QTreeWidgetItem *child : getItemChildren(item))
    {
        all_path_items.remove(child->data(0, Qt::UserRole).toString());
        delete child;
    }
    delete item;
}

void FileTreeWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        if (event->mimeData()->urls().length() > 1) event->ignore();
        else event->acceptProposedAction();
    }
    else event->ignore();
}

void FileTreeWidget::dropEvent(QDropEvent * event)
{
    if(event->mimeData()->hasUrls())
   {
       QUrl url = event->mimeData()->urls().at(0);
       QString path = url.toLocalFile();
       if (addRootPath(path) == false) return;
       emit addDrogItem(path);
   }
   else event->ignore();
}
