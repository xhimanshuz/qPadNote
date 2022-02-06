#include "TodoWindow.h"
#include "Backend.h"
#include <QDebug>

TodoWindow::TodoWindow(std::string _tabName, std::string title, TodoBlockType type, QObject *backend, QWidget *parent) : QWidget(parent), type(type), backend(backend), tabName(_tabName)
{

    dataEngine = DataEngine::getInstance();
    firebase = Firebase::getInstance();

    toBlockMap = (type==TodoBlockType::TODO)?dataEngine->tabBlockMap->find(tabName)->second.first:dataEngine->tabBlockMap->find(tabName)->second.second;
    mainLayout = new QVBoxLayout;

    QHBoxLayout *hbox = new QHBoxLayout;
    titleLable = new QLabel(tr("<h1>> %0 </h1>").arg(title.c_str()));
    titleLable->setStyleSheet(QString("color: #%0; margin-left: 10px; border-bottom: 0.5px solid %0; border-radius: 0px;").arg((type==TodoBlockType::TODO)?"ffd600":"00e676"));
    hbox->addWidget(titleLable, 0, Qt::AlignmentFlag::AlignTop);
    hbox->addStrut(1);
    mainLayout->addLayout(hbox);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    addLineEdit = new QLineEdit;
    addLineEdit->setMaxLength(25);
    addLineEdit->setPlaceholderText("ENTER TASK HERE _");
    connect(addLineEdit, &QLineEdit::returnPressed, [=]{
        QString title = addLineEdit->text();
        if(title.isEmpty())
            return;
        addBlock(title.toStdString(), tabName);

//        dataEngine->writeData();
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
    mainLayout->setContentsMargins(0,0,0,0);

}

TodoWindow::~TodoWindow()
{

}

void TodoWindow::renderUi()
{
    blockVBox = new QVBoxLayout;
    blockVBox->setSpacing(1);
    blockVBox->setContentsMargins(0,0,0,0);
    updateTodoBlocks();
    mainLayout->addLayout(blockVBox);
    mainLayout->addStretch(1);

    mainLayout->setSizeConstraint(QLayout::SetMinimumSize);

    connectSignalSlot();
}

void TodoWindow::connectSignalSlot()
{

}

void TodoWindow::addBlock(std::string title, std::string tabName, int64_t id, int64_t position, std::string subString, uint32_t hash, bool isToDone, int16_t uid)
{
    if(id == 0)
        id = std::chrono::system_clock::now().time_since_epoch().count();
    TodoBlock *block = new TodoBlock(id, tabName, title, subString, hash, isToDone, uid, this);
    setBlockSignals(block);
//    connect(block, &TodoBlock::moveBlock, [&](bool toggle, int64_t id){ moveBlock(toggle, id); });
//    connect(block, &TodoBlock::deleteBlock, this, [=](int64_t id, bool isToDone){
//        dataEngine->deleteBlock(id, tabName);
//        firebase->removeBlock(id, (isToDone)?"toDone":"todo", tabName);
//    });
    toBlockMap->insert({id, block});
    blockVBox->addWidget(block);
    firebase->writeBlock(*block);
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
void TodoWindow::moveBlock(bool toggle, int64_t blockId)
{
    std::shared_ptr<std::map<int64_t, TodoBlock*>> fromBlock;
    std::shared_ptr<std::map<int64_t, TodoBlock*>> toBlock;

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
    auto block = fromBlock->find(blockId);

    block->second->id = std::chrono::system_clock::now().time_since_epoch().count();
    //    it->second.at(0) = block->second->id;
    block->second->isToDone = toggle;
    //    to->insert(std::make_pair(block->second->id, it->second));
    toBlock->insert(std::make_pair(block->second->id, block->second));

    //    from->erase(blockId);
    fromBlock->erase(blockId);
    firebase->moveBlock(std::to_string(blockId), toggle, tabName, std::to_string(block->second->id));


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
    for(auto i=toBlockMap->begin(); i!=toBlockMap->end(); ++i) {
      setBlockSignals(i->second);
    }
}

void TodoWindow::setBlockSignals(TodoBlock *block)
{
  connect(block, &TodoBlock::moveBlock, [&](bool toggle, int64_t id){ moveBlock(toggle, id); });
  connect(block, &TodoBlock::deleteBlock, this, [=](int64_t id, bool isToDone){
    dataEngine->deleteBlock(id, tabName);
    firebase->removeBlock(id, (isToDone)?"toDone":"todo", tabName);
  });
  connect(block, &TodoBlock::updated, [&](TodoBlock *current_block){ firebase->writeBlock(*current_block); });
}

void TodoWindow::updateRender()
{
    ((Backend*)this->backend)->updateTodoWindow(tabName);
}
