
#include "TodoWindow.h"
#include "Backend.h"
#include <QDebug>

TodoWindow::TodoWindow(std::string _tabName, std::string title, TodoBlockType type, QObject *backend, QWidget *parent) : QWidget(parent), type(type), backend(backend), tabName(_tabName)
{

    dataEngine = DataEngine::getInstance();
    toMap = (type==TodoBlockType::TODO)?dataEngine->tabMap->find(tabName)->second.first:dataEngine->tabMap->find(tabName)->second.second;
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
        addBlock(title.toStdString());

        dataEngine->writeData();
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

void TodoWindow::addBlock(std::string title, std::string id, std::string position, std::string subString, bool isToDone)
{
    if(id == "")
        id = std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
    TodoBlock *block = new TodoBlock(id.c_str(), title, subString, isToDone, this);
    connect(block, &TodoBlock::moveBlock, [&](bool toggle, std::string id){ moveBlock(toggle, id); dataEngine->writeData(); });
    connect(block, &TodoBlock::deleteBlock, this, [&](std::string id){
        dataEngine->deleteBlock(id, tabName);
    });

    toMap->insert(std::pair<std::string, std::array<std::string, 6>>(id, { id, position, subString, tabName, title, ((isToDone)?"1":"0")}));
    toBlockMap->insert(std::make_pair(id, block));

    blockVBox->addWidget(block);
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
    std::shared_ptr<std::map<std::string, std::array<std::string, 6> >> from;
    std::shared_ptr<std::map<std::string, std::array<std::string, 6> >> to;
    std::shared_ptr<std::map<std::string, TodoBlock*>> fromBlock;
    std::shared_ptr<std::map<std::string, TodoBlock*>> toBlock;

    if(toggle)
    {
        from = dataEngine->tabMap->find(tabName)->second.first;
        to = dataEngine->tabMap->find(tabName)->second.second;
        fromBlock = dataEngine->tabBlockMap->find(tabName)->second.first;
        toBlock = dataEngine->tabBlockMap->find(tabName)->second.second;
    }
    else
    {
        from = dataEngine->tabMap->find(tabName)->second.second;
        to = dataEngine->tabMap->find(tabName)->second.first;
        fromBlock = dataEngine->tabBlockMap->find(tabName)->second.second;
        toBlock = dataEngine->tabBlockMap->find(tabName)->second.first;
    }

    auto it = from->find(blockId);
    auto block = fromBlock->find(blockId.c_str());

    block->second->id = std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
    it->second.at(0) = block->second->id;
    it->second.at(5) = ((toggle)?"1":"0");
    to->insert(std::make_pair(block->second->id, it->second));
    toBlock->insert(std::make_pair(block->second->id, block->second));

    from->erase(blockId);
    fromBlock->erase(blockId);

    updateRender();
}

void TodoWindow::mapToBlockMap()
{
        for(auto t: *toMap)
        {
            std::string id = t.second.at(0);
            std::string position = t.second.at(1);
            std::string subString = t.second.at(2);
            std::string tabName = t.second.at(3);
            std::string title = t.second.at(4);
            std::string type = t.second.at(5);
            addBlock(title, id, position, subString, (type=="1"));
        }
}

const std::string TodoWindow::getTabName() const
{
    return tabName;
}

void TodoWindow::updateRender()
{
    ((Backend*)this->backend)->updateTodoWindow(tabName);
}
