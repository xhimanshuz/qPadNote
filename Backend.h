#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include "TodoWindow.h"
#include <QSplitter>

class Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(QObject *parent = nullptr);
    void renderUi();

private:
    QVBoxLayout *mainLayout;
    TodoBlock *todoBlock;
    TodoWindow *todoWindow;
    TodoWindow *doneWindow;
    QSplitter *splitter;
};

#endif // BACKEND_H
