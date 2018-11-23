#include "mainpadnote.h"
#include<QDebug>
#include<QIcon>
#include<QPixmap>
MainPadNote::MainPadNote(QRect screenSize, QWidget *parent)
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
    hbox->addWidget(todo = new QLabel);
    hbox->addStretch();
    hbox->addWidget(toolButton);

    tab = new QTabWidget;
    tab->setTabsClosable(true);
    connect(tab, SIGNAL(tabBarDoubleClicked(int)), this, SLOT(renameTab(int)));
    connect(tab, SIGNAL(tabCloseRequested(int)), this, SLOT(removeTab(int)));
    connect(tab, SIGNAL(currentChanged(int)), this, SLOT(setLabel(int)));
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

void MainPadNote::createToolButtonMenu()
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
    about->setShortcut(QKeySequence("F1"));
    connect(about, SIGNAL(triggered()), this, SLOT(aboutPN()));
    toolButtonMenu->addAction(about);

    exit = new QAction(tr("E&xit"));
    exit->setShortcut(QKeySequence("Ctrl+Q"));
    connect(exit, SIGNAL(triggered()), this, SLOT(close()));
    toolButtonMenu->addAction(exit);
}

void MainPadNote::aboutPN()
{
    QMessageBox::about(this, tr("About qPadNote"), "<h3><b>qPadNote 0.1 \nbeta</b></h3>"
                                                   "<p>Qt/C++ based Notepad application."
                                                   "<p><b>Current Maintainer</b><p>"
                                                   "> Himanshu Rastogi <a href=\"https://github.com/xhimanshuz\">xhimanshuz</a>");

}

void MainPadNote::addTab()
{
    tab->addTab(new TextField(fontSize), QString("tab %1").arg(tab->count()+1));
    tab->setCurrentIndex(tab->count()-1);
}

void MainPadNote::removeTab()
{
    if(tab->count()>1)
        return tab->removeTab(tab->currentIndex());
    close();
}
//OverLoaded Function
void MainPadNote::removeTab(int index)
{
    if(tab->count()>1)
        return tab->removeTab(index);
    close();
}

void MainPadNote::renameTab(int index)
{
    bool ok;
    QString str = QInputDialog::getText(this, tr("Enter Tab Text"), tr("Tab Text"), QLineEdit::Normal, tab->tabText(index), &ok);
    if(str != "" && ok)
        tab->setTabText(index, str);
    emit tab->currentChanged(tab->currentIndex());
}

void MainPadNote::setLabel(int index)
{
    todo->setText(QString("<b>Todo</b> - %1").arg(tab->tabText(index)));
}

void MainPadNote::createNotificationMenu()
{
    notificationMenu = new QMenu;
    notificationMenu->addAction(settingAction);
    notificationMenu->addAction(exit);
}

void MainPadNote::configStyleSheet()
{
    this->setWindowOpacity(0.9);
    this->setStyleSheet("QWidget { background-color: black; color: white;}");
    tab->setStyleSheet("QTabBar::tab:selected, QTabBar::tab:hover { color: #9B9B9B; border-bottom-color: black; }");
    this->setStyleSheet("QDialog { background-color: white;}");
    todo->setStyleSheet(" font-size: 25px;");
}

void MainPadNote::_settingDialog()
{
    SettingDialog *sd = new SettingDialog(fontSize, this);
    if(sd->exec() == QDialog::Accepted)
        fontSize = sd->fontSize;
    for(int i=0; i<tab->count(); i++)
    {
        tab->setCurrentIndex(i);
        TextField *tf = static_cast<TextField*>(tab->currentWidget());
        tf->setFontSize(fontSize);
        tf->_setStyleSheet();
    }
}

MainPadNote::~MainPadNote()
{

}
