#ifndef MAINPANNOTE_H
#define MAINPANNOTE_H

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

class MainPanNote : public QWidget
{
    Q_OBJECT

public:
    explicit MainPanNote(QRect screenSize, QWidget *parent = nullptr);
    ~MainPanNote();
private slots:
    void aboutPN();
    void addTab();
    void removeTab();
    void removeTab(int);
    void renameTab(int);
    void _settingDialog();

private:
    void createNotificationMenu();
    void createToolButtonMenu();
    void configStyleSheet();

    int fontSize;

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

#endif // MAINPANNOTE_H
