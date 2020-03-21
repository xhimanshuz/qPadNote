#include "TodoBlock.h"
#include <QDebug>

TodoBlock::TodoBlock(QString title, QWidget *parent) : QWidget(parent), showSub{true}, title(title)
{
    id = QString::number(rand());
    qDebug()<< id;
    mainLayout = new QVBoxLayout;
    renderUi();
    setLayout(mainLayout);
}

void TodoBlock::renderUi()
{
    dateLabel = new QLabel("<i>14.02.2020</i>");
    titleCheckbox = new QCheckBox(title);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->setMargin(2);
    hbox->addWidget(titleCheckbox, 1, Qt::AlignmentFlag::AlignLeft);
    hbox->addStretch();

    hbox->addWidget(dateLabel, 0, Qt::AlignmentFlag::AlignRight);
    deleteToolButton = new QToolButton;
    connect(deleteToolButton, &QToolButton::clicked, [=]{
        this->deleteBlock(this->id);
    });
    deleteToolButton->setIcon(QIcon(":/Data/Data/redDelete.png"));
//    deleteToolButton->setPopupMode(QToolButton::ToolButtonPopupMode::InstantPopup);
    hbox->addWidget(deleteToolButton, 0, Qt::AlignmentFlag::AlignRight);
    mainLayout->addLayout(hbox);

    QPushButton *showHideButton = new QPushButton("...");
    showHideButton->setCheckable(true);
    connect(showHideButton, &QPushButton::toggled, [=](bool toggle){
        showSub = toggle;
        if(showSub)
            subStringTE->show();
        else
        {
            subStringTE->hide();
        }

    });
    mainLayout->addWidget(showHideButton);

    QGroupBox *gb = new QGroupBox("Substring");
    subStringTE = new QTextEdit("");
    mainLayout->addWidget(subStringTE);
    subStringTE->hide();

//    this->setStyleSheet("background: white;");
//    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    mainLayout->setMargin(0);

    connectSignalSlot();
}

void TodoBlock::connectSignalSlot()
{

}


