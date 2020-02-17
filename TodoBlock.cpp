#include "TodoBlock.h"
#include <QDebug>

TodoBlock::TodoBlock(QWidget *parent) : QWidget(parent), showSub{true}
{
    mainLayout = new QVBoxLayout;
    renderUi();
    setLayout(mainLayout);
}

void TodoBlock::renderUi()
{
    dateLabel = new QLabel("<i>14.02.2020</i>");
    titleCheckbox = new QCheckBox(tr("TITLE"));

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->setMargin(2);
    hbox->addWidget(titleCheckbox, 1, Qt::AlignmentFlag::AlignLeft);
    hbox->addStretch();
    hbox->addWidget(dateLabel, 0, Qt::AlignmentFlag::AlignRight);
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
//        qDebug()<< this->size().height()<< this->size().width();

    });
    mainLayout->addWidget(showHideButton);

    QGroupBox *gb = new QGroupBox("Substring");
    subStringTE = new QTextEdit("Specifies the Qt modules that are used by your project. For the value to add for each module, see the module documentation.\n"
                                 "At the C++ implementation level, using a Qt module makes its headers available for inclusion and causes it to be linked to the binary.");
    mainLayout->addWidget(subStringTE);
    subStringTE->hide();

    this->setStyleSheet("background: white;");
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    mainLayout->setMargin(0);
}

QSize TodoBlock::sizeHint() const
{
    return QSize(300, 1);
}

