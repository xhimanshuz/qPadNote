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
#include <random>


class TodoBlock : public QWidget
{
    Q_OBJECT
public:
    explicit TodoBlock(QString tile, QWidget *parent = nullptr);
    void renderUi();
    QCheckBox *titleCheckbox;
    QString title;
    QString id;

protected:
//    QSize sizeHint() const override;

signals:
    void deleteBlock(QString id);
private:
    void connectSignalSlot();

    QVBoxLayout *mainLayout;
    QLabel *substringLabel;
    QLabel *dateLabel;
    QTextEdit *subStringTE;
    QToolButton *deleteToolButton;
    int closeSize;
    bool showSub;
};

#endif // TODOBLOCK_H
