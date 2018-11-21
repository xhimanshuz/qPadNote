#include "textfield.h"
#include<QDebug>

TextField::TextField(QSize s, QWidget *parent): QPlainTextEdit (parent)
{
    size = s;
}

QSize TextField::sizeHint() const
{
    return QSize(size);
}
