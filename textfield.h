#ifndef TEXTFIELD_H
#define TEXTFIELD_H
#include<QPlainTextEdit>

class TextField : public QPlainTextEdit
{
    Q_OBJECT
public:
    TextField(QSize s, QWidget *parent = nullptr);
    QSize sizeHint() const;

private:
    QSize size;
};

#endif // TEXTFIELD_H
