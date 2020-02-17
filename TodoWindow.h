#ifndef TODOWINDOW_H
#define TODOWINDOW_H

#include <QWidget>
#include <QCheckBox>
#include <QVBoxLayout>
#include "TodoDelegate.h"
#include<QScrollArea>

class TodoWindow : public QWidget
{
    Q_OBJECT
public:
    explicit TodoWindow(QWidget *parent = nullptr);
    void renderUi();

private:
    QVBoxLayout *mainLayout;
    QScrollArea *sa;
//    QSize sizeHint() const override;
};

#endif // TODOWINDOW_H
