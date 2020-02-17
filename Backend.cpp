#include "Backend.h"
#include "TodoBlock.h"
#include <QScrollArea>

Backend::Backend(QObject *parent) : QObject(parent)
{
    renderUi();
}

void Backend::renderUi()
{

    splitter = new QSplitter(Qt::Vertical);
    QScrollArea *todoScrollArea = new QScrollArea;
    QScrollArea *doneScrollArea = new QScrollArea;
    doneWindow = new TodoWindow(doneScrollArea);
    todoWindow = new TodoWindow(todoScrollArea);

    todoScrollArea->setWidget(todoWindow);
    doneScrollArea->setWidget(doneWindow);

    splitter->addWidget(todoScrollArea);
    splitter->addWidget(doneScrollArea);

    splitter->show();

}
