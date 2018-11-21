#ifndef MAINPANNOTE_H
#define MAINPANNOTE_H

#include <QWidget>
#include"textfield.h"
#include<QToolButton>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QLabel>
#include<QMenu>
#include<QAction>
#include<QMessageBox>
#include<QSystemTrayIcon>

class MainPanNote : public QWidget
{
    Q_OBJECT

public:
    MainPanNote(QRect screenSize, QWidget *parent = nullptr);
    ~MainPanNote();
private slots:
    void aboutPN();

private:
    void createNotificationMenu();
    void createToolButtonMenu();
    void configStyleSheet();

    QSystemTrayIcon *trayIcon;
    TextField *tf;
    QToolButton *toolButton;
    QMenu *toolButtonMenu;
    QMenu *notificationMenu;
    QAction *exit;
    QAction *about;
};

#endif // MAINPANNOTE_H
