#ifndef TODOWINDOW_H
#define TODOWINDOW_H

#include <QWidget>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLineEdit>
#include <QLabel>


#include "TodoBlock.h"
//#include "Backend.h"

enum class TodoBlockType
{
    TODO,
    DONE
};

class TodoWindow : public QWidget
{
    Q_OBJECT
public:
    explicit TodoWindow(QString Title, TodoBlockType type, QObject *backend, QWidget *parent = nullptr);
    void renderUi();
    void connectSignalSlot();
    void addBlock(QString title);
    void updateTodoBlocks();
    void moveBlock(TodoBlock* block, QVector<QPair<QString, TodoBlock*>> *from, QVector<QPair<QString, TodoBlock*>> *to);
    bool deleteBlock(QString id);

protected:
//    QSize sizeHint() const override;

private:
    void updateRender();
    QVBoxLayout *mainLayout;
    QScrollArea *sa;
    QLineEdit *addLineEdit;
    QLabel *titleLable;
    QVBoxLayout *blockVBox;

    TodoBlockType type;
    QObject *backend;

    static QVector<QPair<QString, TodoBlock*>> *todoMap;
    static QVector<QPair<QString, TodoBlock*>> *todoneMap;
};

#endif // TODOWINDOW_H
