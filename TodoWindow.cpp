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
//    mainLayout->addStretch();

    addLineEdit = new QLineEdit;
    connect(addLineEdit, &QLineEdit::returnPressed, [=]{
        addBlock(addLineEdit->text());
        addLineEdit->clear();
        updateRender();
    });

    renderUi();

    if(type == TodoBlockType::TODO)
    {
        mainLayout->addStretch();
        mainLayout->addWidget(addLineEdit, 1, Qt::AlignmentFlag::AlignBottom);
    }

    this->setLayout(mainLayout);
    mainLayout->setMargin(0);
}

void TodoWindow::renderUi()
{
//    QVBoxLayout *renderUi = new QVBoxLayout;
    QVBoxLayout *vbox = new QVBoxLayout;


    blockVBox = new QVBoxLayout;
    updateTodoBlocks();
    mainLayout->addLayout(blockVBox);
    mainLayout->addStretch(1);

//    vbox->addStretch();

//    mainLayout->addLayout(vbox);

    mainLayout->setSizeConstraint(QLayout::SetMinimumSize);

//    mainLayout->addLayout(renderUi);
    connectSignalSlot();
}

void TodoWindow::connectSignalSlot()
{

}

void TodoWindow::addBlock(QString title, QString subString, bool isToDone)
{
//    auto todoBlock = new TodoBlock(title, QString::number(rand()), subString, isToDone, this);
//    QString id = QString::number(rand());
    QString id = QString::number(std::chrono::system_clock::now().time_since_epoch().count());
    TodoBlock *block = new TodoBlock(id, title, subString, isToDone, this);
    connect(block->titleCheckbox, &QCheckBox::toggled, [=](bool toggle){
        if(toggle)
            moveBlock(block->id, dataEngine->todoMap, dataEngine->toDoneMap, dataEngine->todoBlockMap, dataEngine->toDoneBlockMap);
        else
            moveBlock(block->id, dataEngine->toDoneMap, dataEngine->todoMap, dataEngine->toDoneBlockMap, dataEngine->todoBlockMap);
    });

    if(type == TodoBlockType::TODO)
    {
        dataEngine->todoMap->insert(block->id, QStringList()<< title<< subString<< ((isToDone)?"1":"0"));
        dataEngine->todoBlockMap->insert(block->id, block);
//        blockVBox->addWidget(block);
    }
    else
    {
        dataEngine->toDoneMap->insert(block->id, QStringList()<< title<< subString<< ((isToDone)?"1":"0"));
        dataEngine->toDoneBlockMap->insert(block->id, new TodoBlock(id, title, subString, ((isToDone)?"1":"0"), this));
    }
    updateRender();
}

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

void TodoWindow::updateRender()
{
    ((Backend*)this->backend)->updateTodoWindow();
}
