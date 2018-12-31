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
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>

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

    bool openDB();
    void writeDB();
    void loadDB();
    void initializeFile();

private:
    void createNotificationMenu();
    void createToolButtonMenu();
    void configStyleSheet();
    void errorMsg(const QString, const QString);

    int fontSize;
    int appWidth;

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

    QSqlDatabase *db;
    QSqlQuery *query;
    QString dbName;
};

#endif // MainPadNote_H
