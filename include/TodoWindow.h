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
#include <thread>
#include <chrono>

#include "TodoBlock.h"
#include "dataEngine.h"

//#include "Backend.h"

class TodoWindow : public QWidget
{
    Q_OBJECT
public:
    explicit TodoWindow(std::string _tabName, std::string Title, TodoBlockType type, QObject *backend, QWidget *parent = nullptr);
    ~TodoWindow();
    void renderUi();
    void connectSignalSlot();
    void addBlock(std::string title, std::string id = "", std::string position = "", std::string subString ="", bool isToDone = false);
    void updateTodoBlocks();
    void moveBlock(bool toggle, std::string blockId);
    void mapToBlockMap();
    const std::string getTabName() const;

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
    std::shared_ptr<DataEngine> dataEngine;
    std::string tabName;

    std::shared_ptr<std::map<std::string, std::array<std::string, 6> >> toMap;
    std::shared_ptr<std::map<std::string, TodoBlock*>> toBlockMap;
//    static QVector<QPair<QString, TodoBlock*>> *todoMap;
//    static QVector<QPair<QString, TodoBlock*>> *todoneMap;
};

#endif // TODOWINDOW_H
