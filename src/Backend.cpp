#include "Backend.h"
#include "TodoBlock.h"
#include <QScrollArea>
#include <QDebug>

Backend::Backend(QRect screen, QWidget *parent) : QWidget(parent), screenSize(screen)
{
    mainLayout = new QVBoxLayout;
    dataEngine = DataEngine::getInstance();
    tabToWindowsMap = std::make_shared<std::map<std::string, std::pair<TodoWindow*, TodoWindow*> >>();

    setMouseTracking(true);
    renderUi();
    setLayout(mainLayout);

    createTabByFile();
}

Backend::~Backend()
{

}

void Backend::renderUi()
{
    tabWidget = new QTabWidget;
    connect(tabWidget, &QTabWidget::tabBarDoubleClicked, this, &Backend::renameTab);
    connect(tabWidget, &QTabWidget::tabCloseRequested, this, [this](int index){
        removeTab(index, tabWidget->tabText(tabWidget->currentIndex()).toStdString());
        if(!tabWidget->count())
            createTab();
    });
    tabWidget->setTabPosition(QTabWidget::South);
    tabWidget->setMovable(true);
    tabWidget->setTabsClosable(true);

    this->setGeometry((screenSize.width()-screenSize.width()*0.20), 0, screenSize.width()*0.20, screenSize.height());

    mainLayout->addWidget(tabWidget);
    addTabBar();

}

void Backend::updateTodoWindow(const std::string &tabName)
{

    tabToWindowsMap->find(tabName)->second.first->updateTodoBlocks();
    tabToWindowsMap->find(tabName)->second.second->updateTodoBlocks();
}

QSplitter *Backend::createSplitter(const std::string& tabName)
{
    QSplitter *splitter = new QSplitter(Qt::Vertical);
//    splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);
    QScrollArea *todoScrollArea = new QScrollArea;
    QScrollArea *doneScrollArea = new QScrollArea;
    auto todoWindow = new TodoWindow(tabName, "TODO", TodoBlockType::TODO, this);
    auto doneWindow = new TodoWindow(tabName, "DONE", TodoBlockType::DONE, this);

    tabToWindowsMap->insert(std::make_pair(tabName ,std::make_pair(todoWindow,doneWindow)));

    todoScrollArea->setWidget(todoWindow);
    todoScrollArea->setWidgetResizable(true);
    doneScrollArea->setWidget(doneWindow);
    doneScrollArea->setWidgetResizable(true);

    splitter->addWidget(todoScrollArea);
    splitter->addWidget(doneScrollArea);


    return splitter;
}

void Backend::createTab(std::string name, bool initialCall)
{
    auto count = tabWidget->count();
    std::string tabName = (name == "")?tr("Tab %0").arg(QChar('A'+count)).toStdString():name;

    // Check to create missing tab name sequence
    count = 0;
    while(dataEngine->tabMap->find(tabName) != dataEngine->tabMap->end() && !initialCall)
        tabName = (name == "")?tr("Tab %0").arg(QChar('A'+count++)).toStdString():name;

    dataEngine->createTabMap(tabName);
    tabWidget->addTab(createSplitter(tabName), QString(tabName.c_str()));
    tabWidget->setCurrentIndex(tabWidget->count());
}

void Backend::removeTab(const int index, const std::string &tabName)
{
    tabWidget->removeTab(index);
    dataEngine->removeTabMap(tabName);
}

void Backend::renameTab(int index)
{
    bool ok;
    QString tabName = QInputDialog::getText(this, "Rename Tab Text", "Enter Tab Name", QLineEdit::Normal, tabWidget->tabText(index), &ok);
    if(ok)
        tabWidget->setTabText(index, tabName);
}

void Backend::createTabByFile()
{
    if(!dataEngine->tabMap->size())
        createTab();
    else
    {
        for(auto tab = dataEngine->tabMap->begin(); tab!= dataEngine->tabMap->end(); ++tab)
            createTab(tab->first, true);
    }
}

void Backend::addTabBar()
{
    addTabAction = new QAction("+");
    connect(addTabAction, &QAction::triggered, [this]{
        createTab();
        tabWidget->setCurrentIndex(tabWidget->count());
    });

    delTabAction = new QAction("-");
    connect(delTabAction, &QAction::triggered, [this]{
        removeTab(tabWidget->currentIndex(), tabWidget->tabText(tabWidget->currentIndex()).toStdString());
        if(!tabWidget->count())
            createTab();
    });


    editTabAction = new QAction("e");
    connect(editTabAction, &QAction::triggered, [this]{ renameTab(tabWidget->currentIndex()); });

    moreTabToolButton = new QToolButton;
    moreTabToolButton->setText("...");

    tabToolBar = new QToolBar;
    tabToolBar->addActions(QList<QAction*>()<< addTabAction<< delTabAction<< editTabAction);
    tabToolBar->addWidget(moreTabToolButton);

    mainLayout->addWidget(tabToolBar);
}

void Backend::leaveEvent(QEvent *event)
{
    event->accept();
    DataEngine::getInstance()->writeData();
;}

