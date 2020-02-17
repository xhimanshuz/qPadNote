#ifndef TODODELEGATE_H
#define TODODELEGATE_H

#include <QStyledItemDelegate>
#include "TodoBlock.h"

class TodoDelegate: public QStyledItemDelegate
{
    Q_OBJECT
public:
    TodoDelegate(QObject *parent = nullptr);

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // TODODELEGATE_H
