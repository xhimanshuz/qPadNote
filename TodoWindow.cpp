
#include "TodoWindow.h"
#include "Backend.h"
#include <QDebug>

TodoWindow::TodoWindow(std::string title, TodoBlockType type, QObject *backend, QWidget *parent) : QWidget(parent), type(type), backend(backend)
{
    dataEngine = DataEngine::getInstance();

    mainLayout = new QVBoxLayout;

    QHBoxLayout *hbox = new QHBoxLayout;
    titleLable = new QLabel(tr("<h1> %0 </h1>").arg(title.c_str()));
    hbox->addWidget(titleLable, 0, Qt::AlignmentFlag::AlignTop);
    hbox->addStrut(1);
    mainLayout->addLayout(hbox);

    addLineEdit = new QLineEdit;
    connect(addLineEdit, &QLineEdit::returnPressed, [=]{
        QString title = addLineEdit->text();
        if(title.isEmpty())
            return;
        addBlock(title.toStdString());
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

void TodoWindow::addBlock(std::string title, std::string subString, bool isToDone, std::string id)
{
    if(id == "")
        id = std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
    TodoBlock *block = new TodoBlock(id.c_str(), title, subString, isToDone, this);
    connect(block, &TodoBlock::moveBlock, [=](bool toggle, std::string id){ moveBlock(toggle, id); dataEngine->writeData(); });
    connect(block, &TodoBlock::deleteBlock, this, [=](std::string id){ dataEngine->deleteBlock(id); });

    if(type == TodoBlockType::TODO)
    {
        dataEngine->todoMap->insert(std::pair<std::string, std::array<std::string, 4>>(id, { title, subString, ((isToDone)?"1":"0"), id}));
        dataEngine->todoBlockMap->insert(std::make_pair(id, block));
        blockVBox->addWidget(block);
    }
    else
    {
        dataEngine->toDoneMap->insert(std::pair<std::string, std::array<std::string, 4>>(id, { title, subString, ((isToDone)?"1":"0"), id}));
        dataEngine->toDoneBlockMap->insert(std::make_pair(id, block));
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
            blockVBox->addWidget(i->second);
        }
    }
    else
    {
        for(auto i=dataEngine->toDoneBlockMap->begin(); i!=dataEngine->toDoneBlockMap->end(); ++i)
        {
            blockVBox->addWidget(i->second);
        }
    }
}

// Move todoBlock map to  map
void TodoWindow::moveBlock(bool toggle, std::string blockId)
{
    std::map<std::string, std::array<std::string, 4>> *from;
    std::map<std::string, std::array<std::string, 4>> *to;
    std::map<std::string, TodoBlock *> *fromBlock;
    std::map<std::string, TodoBlock *> *toBlock;

    if(toggle)
    {
        from = dataEngine->todoMap;
        to = dataEngine->toDoneMap;
        fromBlock = dataEngine->todoBlockMap;
        toBlock = dataEngine->toDoneBlockMap;
    }
    else
    {
        from = dataEngine->toDoneMap;
        to = dataEngine->todoMap;
        fromBlock = dataEngine->toDoneBlockMap;
        toBlock = dataEngine->todoBlockMap;
    }

    auto it = from->find(blockId);
    auto block = fromBlock->find(blockId.c_str());

    block->second->id = std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
    to->insert(std::pair<std::string, std::array<std::string, 4>>(block->second->id, it->second));
    toBlock->insert(std::make_pair(block->second->id, block->second));

    from->erase(blockId);
    fromBlock->erase(blockId);

    updateRender();
}

void TodoWindow::mapToBlockMap()
{
    if(type == TodoBlockType::TODO)
    {
        for(auto todo: *dataEngine->todoMap)
        {
            std::string id = todo.second.at(3);
            std::string title = todo.second.at(0);
            std::string subString = todo.second.at(1);
            bool isToDone = false;
            addBlock(title, subString,isToDone, id);
        }
    }
    else
    {
        for(auto toDone: *dataEngine->toDoneMap)
        {
            std::string id = toDone.second.at(3);
            std::string title = toDone.second.at(0);
            std::string subString = toDone.second.at(1);
            bool isToDone = true;
            addBlock(title, subString, isToDone, id);
        }
    }
}

void TodoWindow::updateRender()
{
    ((Backend*)this->backend)->updateTodoWindow();
}
