#ifndef TODOWINDOW_H
#define TODOWINDOW_H

#include <QWidget>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLineEdit>
#include <QLabel>
#include <QAction>
#include <QToolButton>
#include <ctime>
#include <thread>
#include <chrono>

#include "TodoBlock.h"
#include "dataEngine.h"


class TodoWindow : public QWidget
{
    Q_OBJECT
public:
    explicit TodoWindow(std::string _tabName, std::string Title, TodoBlockType type, QObject *backend, QWidget *parent = nullptr);
    ~TodoWindow();
    void renderUi();
    void addBlock(std::string title, std::string tabName, int64_t id = 0, int64_t position = 0, std::string subString ="", uint32_t hash = 0,  bool isToDone = false , int16_t uid = 1001);
    void updateTodoBlocks();
    void moveBlock(bool toggle, int64_t blockId);
    const std::string getTabName() const;
    void setTabName(const std::string &tabName);
    void setSignals();
    void setBlockSignals(TodoBlock *block);

protected:

private:
    void updateRender();
    void setupSignalSlot();
    void setupAction();

    QVBoxLayout *mainLayout;
    QScrollArea *sa;
    QLineEdit *addLineEdit;
    QLabel *titleLable;
    QVBoxLayout *blockVBox;

    TodoBlockType type;
    QObject *backend;
    DataEngine* dataEngine;
    std::string tabName;

    std::shared_ptr<std::map<int64_t, TodoBlock*>> toBlockMap;
    std::shared_ptr<spdlog::logger> log;
    QAction* aboutAction;
    QToolButton *moreToolButton;

    QMenu *menu;
};

#endif // TODOWINDOW_H
