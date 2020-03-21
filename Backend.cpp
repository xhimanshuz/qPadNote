#include "Backend.h"
#include "TodoBlock.h"
#include <QScrollArea>
#include <QDebug>

Backend::Backend(QRect screen, QObject *parent) : QObject(parent), screenSize(screen)
{
    renderUi();
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
    splitter->setGeometry((screenSize.width()-screenSize.width()*0.20), 0, screenSize.width()*0.20, screenSize.height());
//    splitter->setStyleSheet("QWidget{background: black; border: 1px solid whilte;} QLabel{color: white;} QTextEdit{border: 1px solid white; color: white;} QLineEdit{border: 1px solid white; color: white;} QCheckBox{color: white;} QCheckbox::indicator{border: 3px solid blue;}"
//                            "QPushButton{border: 1px solid white; border-radius: 5px; color: white}");
    splitter->show();

}

void Backend::updateTodoWindow()
{
    doneWindow->updateTodoBlocks();
    todoWindow->updateTodoBlocks();
}
