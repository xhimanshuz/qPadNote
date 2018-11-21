#include "mainpannote.h"
#include<QDebug>

MainPanNote::MainPanNote(QRect screenSize, QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setWindowIcon(QIcon(":/clipboard.png"));

    this->setGeometry(screenSize.width(), 0, 300, screenSize.height());
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
    hbox->addWidget(new QLabel("<h1><b>Todo</b></h1>"));
    hbox->addStretch();
    hbox->addWidget(toolButton);
    tf = new TextField(QSize(275, screenSize.height()), this);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(hbox);
    vbox->addWidget(tf);
    vbox->setMargin(0);
    setLayout(vbox);

    configStyleSheet();
}

void MainPanNote::createToolButtonMenu()
{
    toolButtonMenu = new QMenu;

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

void MainPanNote::createNotificationMenu()
{
    notificationMenu = new QMenu;

    notificationMenu->addAction(exit);
}

void MainPanNote::configStyleSheet()
{
    this->setWindowOpacity(0.9);
}

MainPanNote::~MainPanNote()
{

}
