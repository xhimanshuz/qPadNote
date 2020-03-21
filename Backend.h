#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include "TodoWindow.h"
#include "TodoBlock.h"
#include <QSplitter>
#include <QRect>

class Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(QRect screen, QObject *parent = nullptr);
    void renderUi();
    void updateTodoWindow();
    TodoWindow *todoWindow;
    TodoWindow *doneWindow;

private:
    QVBoxLayout *mainLayout;
    TodoBlock *todoBlock;
    QSplitter *splitter;
    QRect screenSize;
};

#endif // BACKEND_H
