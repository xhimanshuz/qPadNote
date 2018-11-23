#ifndef MainPadNote_H
#define MainPadNote_H

#include"settingdialog.h"
#include"textfield.h"

#include <QWidget>
#include<QToolButton>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QLabel>
#include<QMenu>
#include<QAction>
#include<QMessageBox>
#include<QSystemTrayIcon>
#include<QTableWidget>
#include<QInputDialog>

class MainPadNote : public QWidget
{
    Q_OBJECT

public:
    explicit MainPadNote(QRect screenSize, QWidget *parent = nullptr);
    ~MainPadNote();
private slots:
    void aboutPN();
    void addTab();
    void removeTab();
    void removeTab(int);
    void renameTab(int);
    void setLabel(int);
    void _settingDialog();

private:
    void createNotificationMenu();
    void createToolButtonMenu();
    void configStyleSheet();

    int fontSize;

    QLabel *todo;
    QTabWidget *tab;
    QSystemTrayIcon *trayIcon;
    TextField *tf;
    QToolButton *toolButton;
    QMenu *toolButtonMenu;
    QMenu *notificationMenu;
    QAction *settingAction;
    QAction *add;
    QAction *remove;
    QAction *exit;
    QAction *about;
};

#endif // MainPadNote_H
