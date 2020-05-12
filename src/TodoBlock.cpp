#include "TodoBlock.h"
#include <iostream>

TodoBlock::TodoBlock(std::string _id, std::string _title, std::string _subString, bool _toDone, QWidget *parent) : QWidget(parent), showSub{true}, title(_title), id{_id}, subString{_subString}, isToDone{_toDone}
{
    this->setParent(parent);
    std::cout<< id<<std::endl;
    mainLayout = new QVBoxLayout;
    renderUi();
    setLayout(mainLayout);
}

void TodoBlock::renderUi()
{
    const long time = atoll(std::string(id, 0, 10).c_str());

    dateLabel = new QLabel(tr("<i>%0</i>").arg(ctime(&time)));
    titleCheckbox = new QCheckBox(title.c_str());
    titleCheckbox->setChecked(isToDone);
    connect(titleCheckbox, &QCheckBox::toggled, [=](bool toggle){ moveBlock(toggle, id); });

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->setMargin(2);
    hbox->addWidget(titleCheckbox, 1, Qt::AlignmentFlag::AlignLeft);
    hbox->addStretch();

    hbox->addWidget(dateLabel, 0, Qt::AlignmentFlag::AlignRight);
    deleteToolButton = new QToolButton;
    connect(deleteToolButton, &QToolButton::clicked, this, [=]{ emit deleteBlock(id); });
    deleteToolButton->setIcon(QIcon("://redDelete.png"));
    hbox->addWidget(deleteToolButton, 0, Qt::AlignmentFlag::AlignRight);
    mainLayout->addLayout(hbox);

    QPushButton *showHideButton = new QPushButton("...");
    showHideButton->setCheckable(true);
    mainLayout->addWidget(showHideButton);

    subStringTE = new QTextEdit(subString.c_str());
    connect(showHideButton, &QPushButton::toggled, [=](bool toggle){
        showSub = toggle;
        if(showSub)
            subStringTE->show();
        else
        {
            subStringTE->hide();
        }
    });
    mainLayout->addWidget(subStringTE);
    subStringTE->hide();

    mainLayout->setMargin(0);

    connectSignalSlot();
}

void TodoBlock::setSubString(const std::string subString)
{
    subStringTE->setPlainText(subString.c_str());
}

std::string TodoBlock::getSubString()
{
    return subStringTE->toPlainText().toStdString();
}

void TodoBlock::connectSignalSlot()
{

}


