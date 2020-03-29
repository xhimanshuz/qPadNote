#ifndef TODOWINDOW_H
#define TODOWINDOW_H

#include <QWidget>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLineEdit>
#include <QLabel>

#include "dataEngine.h"
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
    void addBlock(QString title, QString subString = "", bool isToDone = false, QString id = "");
    void updateTodoBlocks();
    void moveBlock(QString blockId, QMap<QString, QStringList> *from, QMap<QString, QStringList> *to, QMap<QString, TodoBlock*> *fromBlock, QMap<QString, TodoBlock*> *toBlock);
    bool deleteBlock(QString id);
    void mapToBlockMap();

protected:
//    QSize sizeHint() const override;

private:
    void updateRender();
    QVBoxLayout *mainLayout;
    QScrollArea *sa;
    QLineEdit *addLineEdit;
    QLabel *titleLable;
    QVBoxLayout *blockVBox;
//    QWidget *blockList;

    TodoBlockType type;
    QObject *backend;
    DataEngine *dataEngine;

//    static QVector<QPair<QString, TodoBlock*>> *todoMap;
//    static QVector<QPair<QString, TodoBlock*>> *todoneMap;
};

#endif // TODOWINDOW_H
