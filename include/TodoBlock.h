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
#include <sstream>
#include <iostream>

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
    explicit TodoBlock(int64_t _id, std::string _tid, std::string _title, std::string _subString, uint32_t _hash, bool _isTodone = false, int16_t uid = 1000, QWidget *parent = nullptr);


    int64_t id;
    std::string tid;
    std::string title;
    std::string subString;
    uint32_t hash;
    bool isToDone;
    int16_t uid;
    uint16_t position;
    bool showSub;

    QAction *deleteToolButton;

    void renderUi();
    QCheckBox *titleCheckbox;
    QString getStatusTip();
    uint32_t makeHash();
    bool isHashModified();
protected:
    //    QSize sizeHint() const override;

signals:
    void deleteBlock(int64_t id, bool isToDone);
    void moveBlock(bool toggle, int64_t);
    void updated(TodoBlock* block);
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
    const std::string createdTime;
    std::hash<std::string> strHash;
//    std::shared_ptr<DataEngine> dataEngine;

public:
    const std::string toString();
};

#endif // TODOBLOCK_H
