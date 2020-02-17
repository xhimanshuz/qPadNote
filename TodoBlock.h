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

class TodoBlock : public QWidget
{
    Q_OBJECT
public:
    explicit TodoBlock(QWidget *parent = nullptr);
    void renderUi();

protected:
    QSize sizeHint() const override;

signals:
private:
    QVBoxLayout *mainLayout;
    QLabel *substringLabel;
    QLabel *dateLabel;
    QTextEdit *subStringTE;
    QCheckBox *titleCheckbox;
    bool showSub;
    int closeSize;

};

#endif // TODOBLOCK_H
