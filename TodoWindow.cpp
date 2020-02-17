#include "TodoWindow.h"
#include "TodoBlock.h"

TodoWindow::TodoWindow(QWidget *parent) : QWidget(parent)
{
    mainLayout = new QVBoxLayout;
    renderUi();
    this->setLayout(mainLayout);
    mainLayout->setMargin(0);
    this->setStyleSheet("background: white;");
}

void TodoWindow::renderUi()
{
    QVBoxLayout *vbox = new QVBoxLayout;

    for(auto i=0;i<5;i++)
    {
        auto td = new TodoBlock;
        vbox->addWidget(td);
    }
    vbox->setMargin(5);
    vbox->addStretch();
    mainLayout->addLayout(vbox);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
}

//QSize TodoWindow::sizeHint() const
//{
//    return QSize(300, 300);
//}
