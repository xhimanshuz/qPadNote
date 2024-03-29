#include "Backend.h"
#include "SettingDialog/SettingDialog.h"
#include <QDebug>
#include <QScrollArea>

Backend::Backend(QRect screen, QWidget *parent)
    : QWidget(parent), screenSize(screen) {
  log = spdlog::get("qlog");
  setObjectName("backend");
  setWindowFlags(Qt::WindowType::Dialog |
                 Qt::WindowType::NoDropShadowWindowHint |
                 Qt::WindowType::FramelessWindowHint);
  setWindowFlags(windowFlags() & ~Qt::WindowTitleHint);

  dataEngine = DataEngine::getInstance();

  mainLayout = new QVBoxLayout;
  tabToWindowsMap = std::make_shared<
      std::map<std::string, std::pair<TodoWindow *, TodoWindow *>>>();

  setMouseTracking(true);

    renderUi();
    setLayout(mainLayout);
    createTabByFile();
    setupSystemTrayIcon();
}

Backend::~Backend() {}

void Backend::renderUi() {
  tabWidget = new QTabWidget(this);
  connect(tabWidget, &QTabWidget::tabBarDoubleClicked, this,
          &Backend::renameTab);
  connect(tabWidget, &QTabWidget::tabCloseRequested, this, [this](int index) {
    auto ans =
        QMessageBox::warning(this, "Do you want to Close Tab",
                             "All Tab data will be lost! Do you want that?",
                             QMessageBox::Yes | QMessageBox::No);
    if (QMessageBox::No == ans)
      return;
    removeTab(index,
              tabWidget->tabText(tabWidget->currentIndex()).toStdString());
    if (!tabWidget->count())
      createTab();
  });
  tabWidget->setTabPosition(QTabWidget::South);
  tabWidget->setMovable(true);
  tabWidget->setTabsClosable(true);

  this->setGeometry((screenSize.width() - screenSize.width() * 0.20), 0,
                    screenSize.width() * 0.20, screenSize.height());

  mainLayout->addWidget(tabWidget);
  mainLayout->setContentsMargins(0, 0, 0, 0);
  addTabBar();
  setupSignalSlot();
}

void Backend::updateTodoWindow(const std::string &tabName) {
  _FUNC_LOG_
  tabToWindowsMap->find(tabName)->second.first->updateTodoBlocks();
  tabToWindowsMap->find(tabName)->second.second->updateTodoBlocks();
}

QSplitter *Backend::createSplitter(const std::string &tabName,
                                   QWidget *parent) {
  _FUNC_LOG_
  QSplitter *splitter = new QSplitter(Qt::Vertical, parent);
  //    splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);
  QScrollArea *todoScrollArea = new QScrollArea(splitter);
  QScrollArea *doneScrollArea = new QScrollArea(splitter);
  auto todoWindow = new TodoWindow(tabName, "TODO", TodoBlockType::TODO, this,
                                   todoScrollArea);
  auto doneWindow = new TodoWindow(tabName, "DONE", TodoBlockType::DONE, this,
                                   doneScrollArea);

  tabToWindowsMap->insert(
      std::make_pair(tabName, std::make_pair(todoWindow, doneWindow)));

  todoScrollArea->setWidget(todoWindow);
  todoScrollArea->setWidgetResizable(true);
  doneScrollArea->setWidget(doneWindow);
  doneScrollArea->setWidgetResizable(true);

  splitter->addWidget(todoScrollArea);
  splitter->addWidget(doneScrollArea);

  return splitter;
}

void Backend::createTab(std::string name, bool initialCall) {
  _FUNC_LOG_
  auto count = tabWidget->count();
  std::string tabName =
      (name == "") ? tr("Tab %0").arg(QChar('A' + count)).toStdString() : name;

  // NOTE: Check to create missing tab name sequence
  count = 0;
  while (dataEngine->tabBlockMap->find(tabName) !=
             dataEngine->tabBlockMap->end() &&
         !initialCall) {
    tabName = (name == "")
                  ? tr("Tab %0").arg(QChar('A' + count++)).toStdString()
                  : name;
//    io->addTab(tabName);
  }
  dataEngine->createTabMap(tabName);
  tabWidget->addTab(createSplitter(tabName, tabWidget),
                    QString(tabName.c_str()));
  tabWidget->setCurrentIndex(tabWidget->count());
}

void Backend::removeTab(const int index, const std::string &tabName) {
  _FUNC_LOG_
  tabWidget->removeTab(index);
  dataEngine->removeTabMap(tabName);

//  io->removeTab(tabName);
  //    networkEngine->removeTab(tabName);
}

void Backend::renameTab(int index) {
  _FUNC_LOG_
  auto oldName = tabWidget->tabText(index);
  bool ok;
  QString tabName =
      QInputDialog::getText(this, "Rename Tab Text", "Enter Tab Name",
                            QLineEdit::Normal, oldName, &ok);
  if (ok) {
    tabWidget->setTabText(index, tabName);
    auto todoWindow = std::move(tabToWindowsMap->find(oldName.toStdString())->second.first);
    auto doneWindow = std::move(tabToWindowsMap->find(oldName.toStdString())->second.second);
    todoWindow->setTabName(tabName.toStdString());
    doneWindow->setTabName(tabName.toStdString());

    tabToWindowsMap->insert(std::make_pair( tabName.toStdString(), std::make_pair(std::move(todoWindow), std::move(doneWindow))));
    tabToWindowsMap->erase(oldName.toStdString());

    dataEngine->renameTabMap(oldName.toStdString(), tabName.toStdString());
    dataEngine->writeData();
  }
}

void Backend::createTabByFile() {
  _FUNC_LOG_
  if (!dataEngine->tabBlockMap->size())
    createTab();
  else {
    for (auto tab = dataEngine->tabBlockMap->begin();
         tab != dataEngine->tabBlockMap->end(); ++tab)
      createTab(tab->first, true);
  }
}

void Backend::addTabBar() {
  _FUNC_LOG_
  addTabAction = new QAction("+");
  connect(addTabAction, &QAction::triggered, [this] {
    _FUNC_LOG_
    createTab();
    tabWidget->setCurrentIndex(tabWidget->count());
  });

  delTabAction = new QAction("-");
  connect(delTabAction, &QAction::triggered, [this] {
    _FUNC_LOG_
    if (QMessageBox::No ==
        QMessageBox::warning(this, "Do you want to Close Tab",
                             "All Tab data will be lost! Do you want that?",
                             QMessageBox::Yes | QMessageBox::No))
      return;
    removeTab(tabWidget->currentIndex(),
              tabWidget->tabText(tabWidget->currentIndex()).toStdString());
    if (!tabWidget->count())
      createTab();
  });

  editTabAction = new QAction("e");
  connect(editTabAction, &QAction::triggered,
          [this] { _FUNC_LOG_ renameTab(tabWidget->currentIndex()); });

  moreTabToolButton = new QToolButton;
  moreTabToolButton->setIcon(QIcon("://option.png"));

  closeAction = new QAction("Exit");
  connect(closeAction, &QAction::triggered, [this] { _FUNC_LOG_ this->close(); });

  aboutAction = new QAction("About");

  showAction = new QAction("Show/Hide");
  connect(showAction, &QAction::triggered, [this] {
    _FUNC_LOG_
    if (this->isHidden()) {
      show();
      activateWindow();
    } else
      hide();
  });

  // Need to work on user management
  userInfoButton = new QToolButton(this);
  userInfoButton->setIcon(QIcon("://user.png"));
  connect(userInfoButton, &QToolButton::clicked, [=]() {
    _FUNC_LOG_
  });

  menu = new QMenu;
  menu->addAction(addTabAction);
  menu->addAction(delTabAction);
  menu->addAction(editTabAction);
  menu->addAction(closeAction);
  menu->addAction(showAction);
  menu->addAction(aboutAction);

  moreTabToolButton->setMenu(menu);
  moreTabToolButton->setPopupMode(QToolButton::InstantPopup);

  tabToolBar = new QToolBar;

  tabToolBar->addWidget(new QLabel("<b>Tab Edit: </b>"));
  tabToolBar->addActions(QList<QAction *>()
                         << addTabAction << delTabAction << editTabAction);

  tabToolBar->addSeparator();
  tabToolBar->addWidget(moreTabToolButton);

  QHBoxLayout *userInfoLayout = new QHBoxLayout;
  QWidget *userInfoWidget = new QWidget(this);
  userInfoWidget->setLayout(userInfoLayout);
  userInfoLayout->addStretch();
  userInfoLayout->addWidget(userInfoButton);
  tabToolBar->addWidget(userInfoWidget);

  mainLayout->addWidget(tabToolBar);
  mainLayout->setSpacing(0);
}

void Backend::setupSystemTrayIcon() {
  _FUNC_LOG_
  sysTrayIcon = new QSystemTrayIcon(QIcon("://option.png"));
  connect(sysTrayIcon, &QSystemTrayIcon::activated,
          [this](QSystemTrayIcon::ActivationReason reason) {
            if (reason == QSystemTrayIcon::Trigger) {
              if (this->isActiveWindow() || !isHidden())
                this->hide();
              this->activateWindow();
            }
          });

  sysTrayIcon->showMessage("this is testing mesg", "msg.....");
  sysTrayIcon->setIcon(QIcon("://option.png"));
  sysTrayIcon->setContextMenu(menu);
  sysTrayIcon->show();
}

void Backend::leaveEvent(QEvent *event) {
  _FUNC_LOG_
  event->accept();
//  io->writeData();
}

void Backend::hideEvent(QHideEvent *) {
  _FUNC_LOG_
  //    if(this->isMinimized())
  //        this->activateWindow();
}

void Backend::setupSignalSlot()
{
  connect(aboutAction, &QAction::triggered, this, &Backend::onAboutActionClicked);
}

void Backend::onAboutActionClicked()
{
  SettingDialog sd;
  sd.exec();
}
