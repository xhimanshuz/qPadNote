#ifndef TEXTFIELD_H
#define TEXTFIELD_H
#include<QPlainTextEdit>
#include<QDebug>
#include<QScrollBar>
#include<QFile>


class TextField : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit TextField(int _fontSize, QWidget *parent = nullptr);
    QSize sizeHint() const;
    void _setStyleSheet();
    void saveFile();
    int fontSize;
    void setFontSize(int _fontSize);

signals:
    void focusOutSignal();

protected:
    void focusOutEvent(QFocusEvent *);

private:
    QSize size;
    QFile *file;
};

#endif // TEXTFIELD_H
