#ifndef BACKEND_H
#define BACKEND_H

#include <future>

#include <QHideEvent>
#include <QInputDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QMouseEvent>
#include <QRect>
#include <QSpacerItem>
#include <QSplitter>
#include <QSystemTrayIcon>
#include <QTabWidget>
#include <QToolBar>
#include <QWidget>

#include "AbstractIO.h"
#include "TodoBlock.h"
#include "TodoWindow.h"
#include "dataEngine.h"

class Backend : public QWidget {
  Q_OBJECT
 public:
  explicit Backend(QRect screen, QWidget* parent = nullptr);
  ~Backend();
  void renderUi();
  void updateTodoWindow(const std::string& tabName);

  QSplitter* createSplitter(const std::string& tabName,
                            QWidget* parent = nullptr);
  void createTab(std::string name = "", bool initialCall = false);
  void removeTab(const int index, const std::string& tabName);
  void renameTab(int index);
  void createTabByFile();
  void addTabBar();
  void setupSystemTrayIcon();

 protected:
  void leaveEvent(QEvent* event) override;
  void hideEvent(QHideEvent* event) override;

 private:
  QVBoxLayout* mainLayout;
  QRect screenSize;
  QTabWidget* tabWidget;
  DataEngine* dataEngine;
  AbstractIO* io;

  QAction* addTabAction;
  QAction* delTabAction;
  QAction* editTabAction;
  QAction* closeAction;
  QAction* aboutAction;
  //    QAction *minimizeToTray;
  QToolButton* moreTabToolButton;

  QToolButton* userInfoButton;

  QToolBar* tabToolBar;
  std::shared_ptr<std::map<std::string, std::pair<TodoWindow*, TodoWindow*>>>
      tabToWindowsMap;

  QSystemTrayIcon* sysTrayIcon;
  QMenu* menu;

  QAction* showAction;
  std::shared_ptr<spdlog::logger> log;

  void setupSignalSlot();

 public slots:
      void onAboutActionClicked();
};

#endif  // BACKEND_H
