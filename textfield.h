#ifndef TEXTFIELD_H
#define TEXTFIELD_H
#include<QPlainTextEdit>

class TextField : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit TextField(int _fontSize, QWidget *parent = nullptr);
    QSize sizeHint() const;
    void _setStyleSheet();

private:
    QSize size;
    int fontSize;
};

#endif // TEXTFIELD_H
