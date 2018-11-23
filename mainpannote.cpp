#include "mainpannote.h"
#include<QDebug>

MainPanNote::MainPanNote(QRect screenSize, QWidget *parent)
    : QWidget(parent), fontSize(15)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setWindowIcon(QIcon(":/clipboard.png"));

    this->setGeometry(screenSize.width(), 0, 280, screenSize.height());
    createToolButtonMenu();
    createNotificationMenu();

    trayIcon = new QSystemTrayIcon(QIcon(":/clipboard.png"),this);
    trayIcon->setContextMenu(notificationMenu);
    trayIcon->setVisible(true);

    toolButton = new QToolButton;
    toolButton->setText("...");
    toolButton->setMenu(toolButtonMenu);
    toolButton->setPopupMode(QToolButton::InstantPopup);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addStretch();
    hbox->addWidget(new QLabel("<h1><b>Todo</b></h1>"));
    hbox->addStretch();
    hbox->addWidget(toolButton);

    tab = new QTabWidget;
    tab->setTabsClosable(true);
    connect(tab, SIGNAL(tabBarDoubleClicked(int)), this, SLOT(renameTab(int)));
    connect(tab, SIGNAL(tabCloseRequested(int)), this, SLOT(removeTab(int)));
    tab->setMovable(true);
    tab->setTabPosition(QTabWidget::South);
    tab->addTab(new TextField(fontSize), tr("Tab %1").arg(tab->count() + 1 ));

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(hbox);
    vbox->addWidget(tab);
    vbox->setMargin(0);
    setLayout(vbox);

    configStyleSheet();
}

void MainPanNote::createToolButtonMenu()
{
    toolButtonMenu = new QMenu;

    settingAction = new QAction(tr("&Setting"));
    settingAction->setShortcut(QKeySequence("F2"));
    connect(settingAction, SIGNAL(triggered()), this, SLOT(_settingDialog()));
    toolButtonMenu->addAction(settingAction);

    add = new QAction(tr("&Add"));
    add->setShortcut(QKeySequence("Ctrl+N"));
    connect(add, SIGNAL(triggered()), this , SLOT(addTab()));
    toolButtonMenu->addAction(add);

    remove = new QAction(tr("Remove"));
    remove->setShortcut(QKeySequence("Ctrl+W"));
    connect(remove, SIGNAL(triggered()), this, SLOT(removeTab()));
    toolButtonMenu->addAction(remove);

    about = new QAction(tr("&About"));
    connect(about, SIGNAL(triggered()), this, SLOT(aboutPN()));
    toolButtonMenu->addAction(about);

    exit = new QAction(tr("E&xit"));
    exit->setShortcut(QKeySequence("Ctrl+x"));
    connect(exit, SIGNAL(triggered()), this, SLOT(close()));
    toolButtonMenu->addAction(exit);
}

void MainPanNote::aboutPN()
{
    QMessageBox::about(this, tr("About qPadNote"), "<b>qPadNote 0.1 \nbeta</b>"
                                                   "\n\nQt/C++ bas\ned Notepad application");
}

void MainPanNote::addTab()
{
    tab->addTab(new TextField(fontSize), QString("tab %1").arg(QChar('A'+tab->count())));
}

void MainPanNote::removeTab()
{
    if(tab->count()>1)
        return tab->removeTab(tab->currentIndex());
    close();
}
//OverLoaded Function
void MainPanNote::removeTab(int index)
{
    if(tab->count()>1)
        return tab->removeTab(index);
    close();
}

void MainPanNote::renameTab(int index)
{
    tab->setTabText(index, QInputDialog::getText(this, tr("Enter Tab Text"), tr("Tab Text"), QLineEdit::Normal, tab->tabText(index), nullptr));
}

void MainPanNote::createNotificationMenu()
{
    notificationMenu = new QMenu;
    notificationMenu->addAction(settingAction);
    notificationMenu->addAction(exit);
}

void MainPanNote::configStyleSheet()
{
    this->setWindowOpacity(0.9);
    this->setStyleSheet("QWidget { background-color: black; color: white;}");
    tab->setStyleSheet("QTabBar::tab:selected, QTabBar::tab:hover { color: #9B9B9B; border-bottom-color: black; }");
    this->setStyleSheet("QDialog { background-color: white;}");
}

void MainPanNote::_settingDialog()
{
    SettingDialog *sd = new SettingDialog(fontSize, this);
    if(sd->exec() == QDialog::Accepted)
        fontSize = sd->fontSize;
}

MainPanNote::~MainPanNote()
{

}
