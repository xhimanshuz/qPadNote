#ifndef TODOBLOCK_H
#define TODOBLOCK_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QCheckBox>
#include <QToolButton>
#include <QToolBar>

#include <random>
#include <ctime>
#include <iomanip>

//#include "dataEngine.h"

enum class TodoBlockType
{
    TODO,
    DONE
};


class TodoBlock : public QWidget
{
    Q_OBJECT
public:
    explicit TodoBlock(std::string _id,std::string _title, std::string _subString, bool _isTodone = false, QWidget *parent = nullptr);

    void renderUi();
    QCheckBox *titleCheckbox;
    std::string title;
    std::string id;
    std::string subString;
    bool isToDone;
    QAction *deleteToolButton;

    void setSubString(const std::string subString);
    std::string getSubString();
    QString getStatusTip();
protected:
//    QSize sizeHint() const override;

signals:
    void deleteBlock(std::string id);
    void moveBlock(bool toggle, std::string);
private:
    QWidget *createMoreWidget();
    void connectSignalSlot();

    QToolBar *blockToolBar;
    QAction *showSubButton;
    QVBoxLayout *mainLayout;
    QLabel *substringLabel;
    QLabel *dateLabel;
    QTextEdit *subStringTE;
    QWidget *subWidget;
    QToolButton *subToolButton;
    int closeSize;
    bool showSub;
    const std::string createdTime;

//    std::shared_ptr<DataEngine> dataEngine;
};

#endif // TODOBLOCK_H
