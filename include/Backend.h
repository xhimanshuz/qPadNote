#ifndef BACKEND_H
#define BACKEND_H

#include <QWidget>
#include <QSplitter>
#include <QRect>
#include <QMouseEvent>
#include <QToolBar>
#include <QTabWidget>
#include <QInputDialog>

#include "TodoWindow.h"
#include "TodoBlock.h"
#include "dataEngine.h"

class Backend : public QWidget
{
    Q_OBJECT
public:
    explicit Backend(QRect screen, QWidget *parent = nullptr);
    ~Backend();
    void renderUi();
    void updateTodoWindow(const std::string& tabName);

    QSplitter *createSplitter(const std::string &tabName);
    void createTab(std::string name = "", bool initialCall = false);
    void removeTab(const int index, const std::string &tabName);
    void renameTab(int index);
    void createTabByFile();
    void addTabBar();
protected:
    void leaveEvent(QEvent *event) override;

private:
    QVBoxLayout *mainLayout;
    QRect screenSize;
    QTabWidget *tabWidget;
    std::shared_ptr<DataEngine> dataEngine;

    QAction *addTabAction;
    QAction *delTabAction;
    QAction *editTabAction;
    QToolButton *moreTabToolButton;

    QToolBar *tabToolBar;
    std::shared_ptr<std::map<std::string, std::pair<TodoWindow*, TodoWindow*> >> tabToWindowsMap;

};

#endif // BACKEND_H
