#ifndef TODOWINDOW_H
#define TODOWINDOW_H

#include <QWidget>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLineEdit>
#include <QLabel>
#include <QAction>
#include <ctime>

#include "TodoBlock.h"
#include "dataEngine.h"
#include "NetworkEngine.h"

//#include "Backend.h"

class TodoWindow : public QWidget
{
    Q_OBJECT
public:
    explicit TodoWindow(std::string Title, TodoBlockType type, QObject *backend, QWidget *parent = nullptr);
    ~TodoWindow();
    void renderUi();
    void connectSignalSlot();
    void addBlock(std::string title, std::string subString = "", bool isToDone = false, std::string id = "");
    void updateTodoBlocks();
    void moveBlock(bool toggle, std::string blockId);
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
