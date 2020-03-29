
#include "TodoWindow.h"
#include "Backend.h"
#include <QDebug>

TodoWindow::TodoWindow(QString title, TodoBlockType type, QObject *backend, QWidget *parent) : QWidget(parent), type(type), backend(backend)
{
    dataEngine = DataEngine::getInstance();

    mainLayout = new QVBoxLayout;

    QHBoxLayout *hbox = new QHBoxLayout;
    titleLable = new QLabel(tr("<h1> %0 </h1>").arg(title));
    hbox->addWidget(titleLable, 0, Qt::AlignmentFlag::AlignTop);
    hbox->addStrut(1);
    mainLayout->addLayout(hbox);

    addLineEdit = new QLineEdit;
    connect(addLineEdit, &QLineEdit::returnPressed, [=]{
        QString title = addLineEdit->text();
        if(title.isEmpty())
            return;
        addBlock(title);
        addLineEdit->clear();
    });

    renderUi();

    if(type == TodoBlockType::TODO)
    {
        mainLayout->addStretch();
        mainLayout->addWidget(addLineEdit, 1, Qt::AlignmentFlag::AlignBottom);
    }

    this->setLayout(mainLayout);
    mainLayout->setMargin(0);

    mapToBlockMap();
}

void TodoWindow::renderUi()
{
    blockVBox = new QVBoxLayout;
    updateTodoBlocks();
    mainLayout->addLayout(blockVBox);
    mainLayout->addStretch(1);

    mainLayout->setSizeConstraint(QLayout::SetMinimumSize);

    connectSignalSlot();
}

void TodoWindow::connectSignalSlot()
{

}

void TodoWindow::addBlock(QString title, QString subString, bool isToDone, QString id)
{
    if(id == "")
        id = QString::number(std::chrono::system_clock::now().time_since_epoch().count());
    TodoBlock *block = new TodoBlock(id, title, subString, isToDone, this);
    connect(block->titleCheckbox, &QCheckBox::toggled, [=](bool toggle){
        if(toggle)
            moveBlock(block->id, dataEngine->todoMap, dataEngine->toDoneMap, dataEngine->todoBlockMap, dataEngine->toDoneBlockMap);
        else
            moveBlock(block->id, dataEngine->toDoneMap, dataEngine->todoMap, dataEngine->toDoneBlockMap, dataEngine->todoBlockMap);
        dataEngine->writeData();
    });

    if(type == TodoBlockType::TODO)
    {
        dataEngine->todoMap->insert(id, QStringList()<< title<< subString<< ((isToDone)?"1":"0")<< id);
        dataEngine->todoBlockMap->insert(id, block);
        blockVBox->addWidget(block);
    }
    else
    {
        dataEngine->toDoneMap->insert(id, QStringList()<< title<< subString<< ((isToDone)?"1":"0")<<  id);
        dataEngine->toDoneBlockMap->insert(id, block);
        blockVBox->addWidget(block);
    }
    dataEngine->writeData();
}

// Update the todoBlock and add to theri location either todo or done
void TodoWindow::updateTodoBlocks()
{
    if(type == TodoBlockType::TODO)
    {
        for(auto i=dataEngine->todoBlockMap->begin(); i!=dataEngine->todoBlockMap->end(); ++i)
        {
            blockVBox->addWidget(i.value());
        }
    }
    else
    {
        for(auto i=dataEngine->toDoneBlockMap->begin(); i!=dataEngine->toDoneBlockMap->end(); ++i)
        {
            blockVBox->addWidget(i.value());
        }
    }
}

// Move todoBlock map to  map
void TodoWindow::moveBlock(QString blockId, QMap<QString, QStringList> *from, QMap<QString, QStringList> *to, QMap<QString, TodoBlock *> *fromBlock, QMap<QString, TodoBlock *> *toBlock)
{
    QStringList sList = from->value(blockId);
    auto block = fromBlock->value(blockId);

    block->id = QString::number(std::chrono::system_clock::now().time_since_epoch().count());
    to->insert(block->id, sList);
    toBlock->insert(block->id, block);

    from->remove(blockId);
    fromBlock->remove(blockId);

    updateRender();
}

bool TodoWindow::deleteBlock(QString id)
{
    return false;
}

void TodoWindow::mapToBlockMap()
{
    if(type == TodoBlockType::TODO)
    {
        for(auto todo: *dataEngine->todoMap)
        {
            QString id = todo.at(3);
            QString title = todo.at(0);
            QString subString = todo.at(1);
            bool isToDone = false;
            addBlock(title, subString,isToDone, id);
        }
    }
    else
    {
        for(auto toDone: *dataEngine->toDoneMap)
        {
            QString id = toDone.at(3);
            QString title = toDone.at(0);
            QString subString = toDone.at(1);
            bool isToDone = true;
            addBlock(title, subString, isToDone, id);
        }
    }
}

void TodoWindow::updateRender()
{
    ((Backend*)this->backend)->updateTodoWindow();
}
