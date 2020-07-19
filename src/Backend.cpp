#include "Backend.h"
#include "NetworkEngine.h"
#include <QScrollArea>
#include <QDebug>

Backend::Backend(QRect screen, QWidget *parent) : QWidget(parent), screenSize(screen)
{
    setWindowFlags(Qt::WindowType::Dialog | Qt::WindowType::NoDropShadowWindowHint);
    setWindowFlags(windowFlags() & ~Qt::WindowTitleHint);

    networkEngine = NetworkEngine::getInstance("127.0.0.1", "8000");
    std::thread(std::bind(&NetworkEngine::createConnection, networkEngine)).detach();

    dataEngine = DataEngine::getInstance();

    mainLayout = new QVBoxLayout;
    tabToWindowsMap = std::make_shared<std::map<std::string, std::pair<TodoWindow*, TodoWindow*> >>();

    setMouseTracking(true);
    renderUi();
    setLayout(mainLayout);

    createTabByFile();
    setupSystemTrayIcon();
}

Backend::~Backend()
{

}

void Backend::renderUi()
{
    tabWidget = new QTabWidget;
    connect(tabWidget, &QTabWidget::tabBarDoubleClicked, this, &Backend::renameTab);
    connect(tabWidget, &QTabWidget::tabCloseRequested, this, [this](int index){
        if(QMessageBox::No == QMessageBox::warning(this, "Do you want to Close Tab", "All Tab data will be lost! Do you want that?", QMessageBox::Yes | QMessageBox::No));
            return;
        removeTab(index, tabWidget->tabText(tabWidget->currentIndex()).toStdString());
        if(!tabWidget->count())
            createTab();
    });
    tabWidget->setTabPosition(QTabWidget::South);
    tabWidget->setMovable(true);
    tabWidget->setTabsClosable(true);

    this->setGeometry((screenSize.width()-screenSize.width()*0.20), 0, screenSize.width()*0.20, screenSize.height());

    mainLayout->addWidget(tabWidget);
    mainLayout->setMargin(0);
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
    auto oldName = tabWidget->tabText(index);
    bool ok;
    QString tabName = QInputDialog::getText(this, "Rename Tab Text", "Enter Tab Name", QLineEdit::Normal, oldName, &ok);
    if(ok)
    {
        tabWidget->setTabText(index, tabName);
        dataEngine->renameTabMap(oldName.toStdString(), tabName.toStdString());
        auto todoWindow = std::move(tabToWindowsMap->find(oldName.toStdString())->second.first);
        auto doneWindow = std::move(tabToWindowsMap->find(oldName.toStdString())->second.second);
        todoWindow->setTabName(tabName.toStdString());
        doneWindow->setTabName(tabName.toStdString());

        tabToWindowsMap->insert(std::make_pair(tabName.toStdString(), std::make_pair(std::move(todoWindow), std::move(doneWindow))));
        tabToWindowsMap->erase(oldName.toStdString());
    }

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
        if(QMessageBox::No == QMessageBox::warning(this, "Do you want to Close Tab", "All Tab data will be lost! Do you want that?", QMessageBox::Yes | QMessageBox::No));
            return;
        removeTab(tabWidget->currentIndex(), tabWidget->tabText(tabWidget->currentIndex()).toStdString());
        if(!tabWidget->count())
            createTab();
    });


    editTabAction = new QAction("e");
    connect(editTabAction, &QAction::triggered, [this]{ renameTab(tabWidget->currentIndex()); });

    moreTabToolButton = new QToolButton;
    moreTabToolButton->setIcon(QIcon("://option.png"));

    closeAction = new QAction("Exit");
    connect(closeAction, &QAction::triggered, [this]{
        this->close();
    });

    showAction = new QAction("Show/Hide");
    connect(showAction, &QAction::triggered, [this]{
        if(this->isHidden())
        {
            show();
            activateWindow();
        }
        else
            hide();
    });

    menu =  new QMenu;
    menu->addAction(addTabAction);
    menu->addAction(delTabAction);
    menu->addAction(editTabAction);
    menu->addAction(closeAction);
    menu->addAction(showAction);
//    menu->addAction(minimizeToTray);


    moreTabToolButton->setMenu(menu);
    moreTabToolButton->setPopupMode(QToolButton::InstantPopup);


    tabToolBar = new QToolBar;
    tabToolBar->addWidget(new QLabel("<b>Tab Edit: </b>"));
    tabToolBar->addActions(QList<QAction*>()<< addTabAction<< delTabAction<< editTabAction);

    tabToolBar->addSeparator();
    tabToolBar->addWidget(moreTabToolButton);

    mainLayout->addWidget(tabToolBar);
    mainLayout->setSpacing(0);
}

void Backend::setupSystemTrayIcon()
{
    sysTrayIcon = new QSystemTrayIcon(QIcon("://option.png"));
    connect(sysTrayIcon, &QSystemTrayIcon::activated, [this](QSystemTrayIcon::ActivationReason reason){
        if(reason == QSystemTrayIcon::Trigger)
        {
            if(this->isActiveWindow() || !isHidden())
                this->hide();
            this->activateWindow();
        }
    });

    sysTrayIcon->showMessage("this is testing mesg", "msg.....");
    sysTrayIcon->setIcon(QIcon("://option.png"));
    sysTrayIcon->setContextMenu(menu);
    sysTrayIcon->show();
}

void Backend::leaveEvent(QEvent *event)
{
    event->accept();
    DataEngine::getInstance()->writeData();
}

void Backend::hideEvent(QHideEvent *)
{
//    if(this->isMinimized())
//        this->activateWindow();
}

