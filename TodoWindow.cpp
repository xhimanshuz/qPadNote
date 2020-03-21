#include "TodoWindow.h"
#include "Backend.h"
#include <QDebug>
#include <QRect>

QVector<QPair<QString, TodoBlock*>> *TodoWindow::todoMap = new QVector<QPair<QString, TodoBlock*>>;
QVector<QPair<QString, TodoBlock*>> *TodoWindow::todoneMap = new QVector<QPair<QString, TodoBlock*>>;

TodoWindow::TodoWindow(QString title, TodoBlockType type, QObject *backend, QWidget *parent) : QWidget(parent), type(type), backend(backend)
{
    mainLayout = new QVBoxLayout;
    QHBoxLayout *hbox = new QHBoxLayout;
    titleLable = new QLabel(tr("<h1> %0 </h1>").arg(title));
    hbox->addWidget(titleLable);
    hbox->addStrut(1);
    mainLayout->addLayout(hbox);
    renderUi();
    this->setLayout(mainLayout);
    mainLayout->setMargin(0);
}

void TodoWindow::renderUi()
{
    QVBoxLayout *vbox = new QVBoxLayout;
    blockVBox = new QVBoxLayout;

    vbox->addLayout(blockVBox);
    vbox->addStretch();
    addLineEdit = new QLineEdit;

    if(type == TodoBlockType::TODO)
    {
        vbox->addStretch();
        vbox->addWidget(addLineEdit, 1, Qt::AlignmentFlag::AlignBottom);
    }

    mainLayout->addLayout(vbox);

    mainLayout->setSizeConstraint(QLayout::SetMinimumSize);

    connectSignalSlot();
}

void TodoWindow::connectSignalSlot()
{
    connect(addLineEdit, &QLineEdit::returnPressed, [=]{
        addBlock(addLineEdit->text());
        addLineEdit->clear();
    });
}

void TodoWindow::addBlock(QString title)
{
    if(type == TodoBlockType::TODO)
    {
        auto todoBlock = new TodoBlock(title, this);
        connect(todoBlock, &TodoBlock::deleteBlock, [=](QString id){
            if(deleteBlock(id))
                qDebug()<<"Deleted"<< id;
            else
                qDebug()<<"Not Found"<< id;
            updateRender();
        });

        connect(todoBlock->titleCheckbox, &QCheckBox::toggled, [=](bool toggle){
            if(toggle)
                moveBlock(todoBlock, todoMap, todoneMap);
            else
                moveBlock(todoBlock, todoneMap, todoMap);

        });
        QPair<QString, TodoBlock*> pair(QString(todoBlock->id), todoBlock);
        todoMap->append(pair);
    }
    else
    {
        auto todoBlock = new TodoBlock(title);
        todoBlock->titleCheckbox->setChecked(true);
        connect(todoBlock->titleCheckbox, &QCheckBox::toggled, [=](bool toggle){
            if(!toggle)
                moveBlock(todoBlock, todoneMap, todoMap);
            else
                moveBlock(todoBlock, todoMap, todoneMap);
        });
        QPair<QString, TodoBlock*> pair(title, todoBlock);
        todoneMap->append(pair);
    }
    updateTodoBlocks();
}

void TodoWindow::updateTodoBlocks()
{
    if(type == TodoBlockType::TODO)
    {
        for(auto i=todoMap->begin(); i!=todoMap->end(); ++i)
        {
            blockVBox->addWidget(i->second);
        }
    }
    else
    {
        for(auto i=todoneMap->begin(); i!=todoneMap->end(); ++i)
        {
            blockVBox->addWidget(i->second);
        }
    }
}

void TodoWindow::moveBlock(TodoBlock *block, QVector<QPair<QString, TodoBlock*>> *from, QVector<QPair<QString, TodoBlock*>> *to)
{
    QPair<QString, TodoBlock*> pair(QString(block->id), block);
    from->remove(from->indexOf(pair));
    to->push_back(pair);
    updateRender();
}

bool TodoWindow::deleteBlock(QString id)
{
    for(auto i=todoMap->begin(); i!=todoMap->end(); ++i)
    {
        if(i->first == id)
        {
            todoMap->remove(todoMap->indexOf(*i));
            i->second->deleteLater();
            return true;
        }
    }
    for(auto i=todoneMap->begin(); i!=todoneMap->end(); ++i)
    {
        if(i->first == id)
        {
            todoneMap->remove(todoMap->indexOf(*i));
            i->second->deleteLater();
            return true;
        }
    }
    return false;
}

void TodoWindow::updateRender()
{
    ((Backend*)this->backend)->updateTodoWindow();
}

//QSize TodoWindow::sizeHint() const
//{
//    return QSize(350, 1);
//}

