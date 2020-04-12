#include "Backend.h"
#include "TodoBlock.h"
#include <QScrollArea>
#include <QDebug>

Backend::Backend(QRect screen, QWidget *parent) : QWidget(parent), screenSize(screen)
{
    mainLayout = new QVBoxLayout;
    renderUi();
    setLayout(mainLayout);
}

Backend::~Backend()
{
    auto k = 5;
}

void Backend::renderUi()
{
    splitter = new QSplitter(Qt::Vertical);
//    splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);
    QScrollArea *todoScrollArea = new QScrollArea;
    QScrollArea *doneScrollArea = new QScrollArea;
    todoWindow = new TodoWindow("TODO", TodoBlockType::TODO, this);
    doneWindow = new TodoWindow("DONE", TodoBlockType::DONE, this);


    todoScrollArea->setWidget(todoWindow);
    todoScrollArea->setWidgetResizable(true);
    doneScrollArea->setWidget(doneWindow);
    doneScrollArea->setWidgetResizable(true);

    splitter->addWidget(todoScrollArea);
    splitter->addWidget(doneScrollArea);

//    qDebug()<< screenSize.width()*.2;
    this->setGeometry((screenSize.width()-screenSize.width()*0.20), 0, screenSize.width()*0.20, screenSize.height());
//    splitter->setStyleSheet("QWidget{background: black; border: 1px solid whilte;} QLabel{color: white;} QTextEdit{border: 1px solid white; color: white;} QLineEdit{border: 1px solid white; color: white;} QCheckBox{color: white;} QCheckbox::indicator{border: 3px solid blue;}"
//                            "QPushButton{border: 1px solid white; border-radius: 5px; color: white}");
//    splitter->show();
    mainLayout->addWidget(splitter);

    splitter->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowDoesNotAcceptFocus);

}

void Backend::updateTodoWindow()
{
    doneWindow->updateTodoBlocks();
    todoWindow->updateTodoBlocks();
}
