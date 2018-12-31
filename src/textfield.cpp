#include "includes/textfield.h"

TextField::TextField(int _fontSize, QWidget *parent): QPlainTextEdit (parent)
{
    fontSize = _fontSize;
    _setStyleSheet();
}

QSize TextField::sizeHint() const
{
    return QSize(size);
}

void TextField::_setStyleSheet()
{
//    setStyleSheet("QScrollBar:vertical {border: 2px solid black; background: black; width: 2px;}");
/*                  " QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical { background: white; width: 1px; height: 1px; border: 2px solid grey;}")*/;
    setStyleSheet(tr("QPlainTextEdit {background-color: black; font: bold %1px; border-color: red;} QScrollBar:vertical {border: 2px solid black; background: black; width: 2px;}").arg(QString::number(fontSize)));
}

void TextField::setFontSize(int _fontSize)
{
    fontSize = _fontSize;
}

void TextField::focusOutEvent(QFocusEvent *)
{
    emit focusOutSignal();
}
