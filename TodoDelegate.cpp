#include "TodoDelegate.h"

TodoDelegate::TodoDelegate(QObject *parent):QStyledItemDelegate(parent)
{

}

QWidget *TodoDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    TodoBlock *todoBlock = new TodoBlock(parent);

    return todoBlock;
}

void TodoDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    TodoBlock *block = static_cast<TodoBlock*>(editor);
}

void TodoDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    TodoBlock *block = static_cast<TodoBlock*>(editor);

    model->setData(index, 0, Qt::DisplayRole);
}

void TodoDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
