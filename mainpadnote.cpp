#include "mainpadnote.h"
#include<QDebug>

MainPadNote::MainPadNote(QRect screenSize, QWidget *parent)
    : QWidget(parent), fontSize(15), dbName("qPN.db"), appWidth(0)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setWindowIcon(QIcon(":/clipboard.png"));
    fontSize = static_cast<int>(screenSize.width()*0.01); // 10% of screen size
    appWidth = static_cast<int>(screenSize.width()*0.20); //20% of Screen size
    this->setGeometry(screenSize.width()-appWidth, 0, appWidth, screenSize.height());
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

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(hbox);
    vbox->addWidget(tab);
    vbox->setMargin(0);
    setLayout(vbox);

    configStyleSheet();
    initializeFile();
}

void MainPadNote::createToolButtonMenu()
{
    toolButtonMenu = new QMenu;

    settingAction = new QAction(tr("&Setting - F2"));
    settingAction->setShortcut(QKeySequence("F2"));
    connect(settingAction, SIGNAL(triggered()), this, SLOT(_settingDialog()));
    toolButtonMenu->addAction(settingAction);

    save = new QAction(tr("&Save - Ctrl+S"));
    save->setShortcut(QKeySequence("Ctrl+S"));
    connect(save, SIGNAL(triggered()), this, SLOT(writeDB()));
    toolButtonMenu->addAction(save);

    load = new QAction(tr("&Load - Ctrl+O"));
    load->setShortcut(QKeySequence("Ctrl+O"));
    connect(load, SIGNAL(triggered()), this, SLOT(loadDB()));
    toolButtonMenu->addAction(load);

    add = new QAction(tr("&Add - Ctrl+N"));
    add->setShortcut(QKeySequence("Ctrl+N"));
    connect(add, SIGNAL(triggered()), this , SLOT(addTab()));
    toolButtonMenu->addAction(add);

    remove = new QAction(tr("Remove - Ctrl+W"));
    remove->setShortcut(QKeySequence("Ctrl+W"));
    connect(remove, SIGNAL(triggered()), this, SLOT(removeTab()));
    toolButtonMenu->addAction(remove);

    about = new QAction(tr("&About - F1"));
    about->setShortcut(QKeySequence("F1"));
    connect(about, SIGNAL(triggered()), this, SLOT(aboutPN()));
    toolButtonMenu->addAction(about);

    exit = new QAction(tr("E&xit - Ctrl+Q"));
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
    TextField *tf = new TextField(fontSize);
    connect(tf, &TextField::focusOutSignal, this, &MainPadNote::writeDB);
    tab->addTab(tf, QString("Task %1").arg(tab->count()+1));
    tab->setCurrentIndex(tab->count()-1);
}

void MainPadNote::removeTab()
{
    if(!openDB())
        return;
    QString tabName = tab->tabText(tab->currentIndex());
    query->prepare("DELETE FROM `tab` WHERE `tabName` = ?");
    query->addBindValue(tabName);
    if(!query->exec())
        errorMsg("Error in Deleting" + tabName, query->lastError().text());
    tab->removeTab(tab->currentIndex());
    if(tab->count()<1)
        addTab();
    db->close();
}

//OverLoaded Function for perticular Tab Removal or Non-focused tab
void MainPadNote::removeTab(int index)
{
    if(!openDB())
        return;
    QString tabName = tab->tabText(index);
    query->prepare("DELETE FROM `tab` WHERE `tabName` = ?");
    query->addBindValue(tabName);
    if(!query->exec())
        errorMsg("Error in Deleting" + tabName, query->lastError().text());
    tab->removeTab(index);
    if(tab->count()<1)
        addTab();
    db->close();
}

void MainPadNote::renameTab(int index)
{
    if(!openDB())
        return;
    bool ok;
    QString old = tab->tabText(index);
    QString str = QInputDialog::getText(this, tr("Enter Tab Text"), tr("Tab Text"), QLineEdit::Normal, tab->tabText(index), &ok);
    if(str != "" && ok)
        tab->setTabText(index, str);
    emit tab->currentChanged(tab->currentIndex());
    query->prepare("UPDATE `tab` set `tabName` = ? WHERE `tabName` = ?");
    query->bindValue(0, str);
    query->bindValue(1, old);
    if(!query->exec())
        errorMsg("Error in updating", query->lastError().text());
    db->close();
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
    tab->setStyleSheet("QTabBar::tab:hover, QTabBar::tab:selected { background-color: #292929; border-top-color: black; } QTabBar::tab {background-color: black; font-size: 15px; font-family: \"New Century Schoolbook\" }");
    this->setStyleSheet("QDialog { background-color: white;}");
    todo->setStyleSheet(" font-size: 25px;");
}

void MainPadNote::errorMsg(const QString title, const QString msg)
{
    QMessageBox::warning(this, title, msg, QMessageBox::Ok);
}

void MainPadNote::_settingDialog()
{
    SettingDialog *sd = new SettingDialog(fontSize, this);
    if(sd->exec() == QDialog::Accepted)
        fontSize = sd->fontSize;
    int curIndex = tab->currentIndex();
    for(int i=0; i<tab->count(); i++)
    {
        tab->setCurrentIndex(i);
        TextField *tf = static_cast<TextField*>(tab->currentWidget());
        tf->setFontSize(fontSize);
        tf->_setStyleSheet();
    }
    tab->setCurrentIndex(curIndex);
}

bool MainPadNote::openDB()
{
    if(!db->open())
    {
        errorMsg("Error in connecting database", db->lastError().text());
        return false;
    }
    return true;
}

void MainPadNote::initializeFile()
{
//    dir = new QDir;
//    dbName = dir->homePath() + "/qPadNote.db";
    db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    db->setDatabaseName(dbName);

    if(!openDB())
        return;
    query = new QSqlQuery;
    query->exec("CREATE TABLE IF NOT EXISTS `tab` (`tabName` TEXT UNIQUE PRIMARY KEY, `tabString` TEXT)");
    QStringList list;
    query->exec("SELECT `tabName` FROM `tab` ORDER BY `tabName`");
    while (query->next())
    {
        list << query->value(0).toString();
    }
    if(list.isEmpty())
    {
        TextField *tf = new TextField(fontSize);
        connect(tf, &TextField::focusOutSignal, this, &MainPadNote::writeDB);
        tab->addTab(tf, QString("Task 1"));
        return;
    }
    int i = 0;
    do{
        tab->setCurrentIndex(i);
        addTab();
        tab->setTabText(i, list[i]);
        i++;
    }
    while(i<list.count());

    loadDB();
    tab->setCurrentIndex(0);
    db->close();
}

void MainPadNote::writeDB()
{
    if(!openDB())
        return;
    int current = tab->currentIndex();
    for(int i=0; i< tab->count(); i++)
    {
        tab->setCurrentIndex(i);
        QString tabName = tab->tabText(i);
        TextField *tf = static_cast<TextField*>(tab->currentWidget());
        QString tabString = tf->toPlainText();
        query->prepare("insert into `tab` (`tabName`) values (?)");
        query->addBindValue(tabName);
        query->exec();
        query->prepare("update `tab` set `tabString`= ? where `tabName`= ?");
        query->bindValue(0, tabString);
        query->bindValue(1, tabName);
        if(!query->exec())
            errorMsg("Error in Inserting", query->lastError().text());
    }
    tab->setCurrentIndex(current);
    db->commit();
}

void MainPadNote::loadDB()
{
    if(!openDB())
        return;
    for(int i=0;i<tab->count(); i++)
    {
        tab->setCurrentIndex(i);
        QString tabName = tab->tabText(i);
        TextField *tf = static_cast<TextField*>(tab->currentWidget());
        query->prepare("SELECT `tabString` FROM `tab` WHERE `tabName` = ?");
        query->bindValue(0, tabName);
        if(!query->exec())
        {
            errorMsg("Error in loading database", query->lastError().text());
            return;
        }
        if(query->next())
            tf->setPlainText(query->value(0).toString());
        else
            errorMsg("Error in query", query->lastError().text());
    }
    db->close();
}

MainPadNote::~MainPadNote()
{
    writeDB();
    db->close();
}
