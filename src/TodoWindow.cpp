#include "NetworkEngine.h"
#include "TodoWindow.h"
#include "Backend.h"
#include <QDebug>

TodoWindow::TodoWindow(std::string _tabName, std::string title, TodoBlockType type, QObject *backend, QWidget *parent) : QWidget(parent), type(type), backend(backend), tabName(_tabName)
{
    networkEngine = NetworkEngine::getInstance();

    dataEngine = DataEngine::getInstance();

    toBlockMap = (type==TodoBlockType::TODO)?dataEngine->tabBlockMap->find(tabName)->second.first:dataEngine->tabBlockMap->find(tabName)->second.second;
    mainLayout = new QVBoxLayout;

    QHBoxLayout *hbox = new QHBoxLayout;
    titleLable = new QLabel(tr("<h1> %0 </h1>").arg(title.c_str()));
    hbox->addWidget(titleLable, 0, Qt::AlignmentFlag::AlignTop);
    hbox->addStrut(1);
    mainLayout->addLayout(hbox);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);

    addLineEdit = new QLineEdit;
    addLineEdit->setMaxLength(25);
    connect(addLineEdit, &QLineEdit::returnPressed, [=]{
        QString title = addLineEdit->text();
        if(title.isEmpty())
            return;
        addBlock(title.toStdString(), tabName);

        dataEngine->writeData();
        addLineEdit->clear();
    });

    renderUi();

    if(type == TodoBlockType::TODO)
    {
        mainLayout->addStretch();
        mainLayout->addWidget(addLineEdit, 1, Qt::AlignmentFlag::AlignBottom);
    }
    setSignals();

    this->setLayout(mainLayout);
    mainLayout->setMargin(0);

}

TodoWindow::~TodoWindow()
{

}

void TodoWindow::renderUi()
{
    blockVBox = new QVBoxLayout;
    blockVBox->setSpacing(1);
    blockVBox->setMargin(0);
    updateTodoBlocks();
    mainLayout->addLayout(blockVBox);
    mainLayout->addStretch(1);

    mainLayout->setSizeConstraint(QLayout::SetMinimumSize);

    connectSignalSlot();
}

void TodoWindow::connectSignalSlot()
{

}

void TodoWindow::addBlock(std::string title, std::string tabName, std::string id, std::string position, std::string subString, std::string hash, bool isToDone)
{
    if(id == "")
        id = std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
    TodoBlock *block = new TodoBlock(id.c_str(), tabName, title, subString, hash, isToDone, this);
    connect(block, &TodoBlock::moveBlock, [&](bool toggle, std::string id){ moveBlock(toggle, id); dataEngine->writeData(); });
    connect(block, &TodoBlock::deleteBlock, this, [=](std::string id){
        dataEngine->deleteBlock(id, tabName);
        networkEngine->removeBlock(std::atoll(id.c_str()));
    });

    toBlockMap->insert(std::make_pair(id, block));

    blockVBox->addWidget(block);
    networkEngine->sendBlock(*block);
}



// Update the todoBlock and add to theri location either todo or done
void TodoWindow::updateTodoBlocks()
{
    for(auto i=toBlockMap->begin(); i!=toBlockMap->end(); ++i)
    {
        blockVBox->addWidget(i->second);
    }
}

// Move todoBlock map to  map
void TodoWindow::moveBlock(bool toggle, std::string blockId)
{
    std::shared_ptr<std::map<std::string, TodoBlock*>> fromBlock;
    std::shared_ptr<std::map<std::string, TodoBlock*>> toBlock;

    if(toggle)
    {
        fromBlock = dataEngine->tabBlockMap->find(tabName)->second.first;
        toBlock = dataEngine->tabBlockMap->find(tabName)->second.second;
    }
    else
    {
        fromBlock = dataEngine->tabBlockMap->find(tabName)->second.second;
        toBlock = dataEngine->tabBlockMap->find(tabName)->second.first;
    }

    //    auto it = from->find(blockId);
    auto block = fromBlock->find(blockId.c_str());

    block->second->id = std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
    //    it->second.at(0) = block->second->id;
    block->second->isToDone = toggle;
    //    to->insert(std::make_pair(block->second->id, it->second));
    toBlock->insert(std::make_pair(block->second->id, block->second));

    //    from->erase(blockId);
    fromBlock->erase(blockId);

    updateRender();
}

const std::string TodoWindow::getTabName() const
{
    return tabName;
}

void TodoWindow::setTabName(const std::string &_tabName)
{
    this->tabName = _tabName;
}

void TodoWindow::setSignals()
{
    for(auto i=toBlockMap->begin(); i!=toBlockMap->end(); ++i)
    {

        connect(i->second, &TodoBlock::moveBlock, [&](bool toggle, std::string id){
            moveBlock(toggle, id);
            dataEngine->writeData();
        });
        connect(i->second, &TodoBlock::deleteBlock, this, [=](std::string id){
            dataEngine->deleteBlock(id, tabName);
            networkEngine->removeBlock(std::atoll(id.c_str()));
        });
    }
}

void TodoWindow::updateRender()
{
    ((Backend*)this->backend)->updateTodoWindow(tabName);
}
