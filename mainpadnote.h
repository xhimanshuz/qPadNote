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
#include<QTextStream>
#include<QFile>
#include<QDir>

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
    void writeFiles();
    void loadFiles();
    void initializeFile();

private:
    void delFiles(const QString fileName);
    void createNotificationMenu();
    void createToolButtonMenu();
    void configStyleSheet();

    int fontSize;

    QString fileIO;
    QDir *dir;
    QLabel *todo;
    QTabWidget *tab;
    QSystemTrayIcon *trayIcon;
    TextField *tf;
    QToolButton *toolButton;
    QMenu *toolButtonMenu;
    QMenu *notificationMenu;
    QAction *settingAction;
    QAction *save;
    QAction *load;
    QAction *add;
    QAction *remove;
    QAction *exit;
    QAction *about;
};

#endif // MainPadNote_H
