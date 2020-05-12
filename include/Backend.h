#ifndef BACKEND_H
#define BACKEND_H

#include <QWidget>
#include <QSplitter>
#include <QRect>
#include <QMouseEvent>

#include "TodoWindow.h"
#include "TodoBlock.h"

class Backend : public QWidget
{
    Q_OBJECT
public:
    explicit Backend(QRect screen, QWidget *parent = nullptr);
    ~Backend();
    void renderUi();
    void updateTodoWindow();
    TodoWindow *todoWindow;
    TodoWindow *doneWindow;

protected:
    void leaveEvent(QEvent *event) override;

private:
    QVBoxLayout *mainLayout;
    TodoBlock *todoBlock;
    QSplitter *splitter;
    QRect screenSize;
};

#endif // BACKEND_H
