#include "TodoBlock.h"
#include <QDebug>

TodoBlock::TodoBlock(QString _id, QString _title, QString _subString, bool _toDone, QWidget *parent) : QWidget(parent), showSub{true}, title(_title), id{_id}, subString{_subString}, isToDone{_toDone}
{
    qDebug()<< id;
    mainLayout = new QVBoxLayout;
    renderUi();
    setLayout(mainLayout);
}

void TodoBlock::renderUi()
{
    dateLabel = new QLabel("<i>14.02.2020</i>");
    titleCheckbox = new QCheckBox(title);
    titleCheckbox->setChecked(isToDone);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->setMargin(2);
    hbox->addWidget(titleCheckbox, 1, Qt::AlignmentFlag::AlignLeft);
    hbox->addStretch();

    hbox->addWidget(dateLabel, 0, Qt::AlignmentFlag::AlignRight);
    deleteToolButton = new QToolButton;
    connect(deleteToolButton, &QToolButton::clicked, [=]{
    });
    deleteToolButton->setIcon(QIcon(":/Data/Data/redDelete.png"));
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
    subStringTE = new QTextEdit(subString);
    mainLayout->addWidget(subStringTE);
    subStringTE->hide();

    mainLayout->setMargin(0);

    connectSignalSlot();
}

void TodoBlock::setSubString(const QString subString)
{
    subStringTE->setPlainText(subString);
}

QString TodoBlock::getSubString()
{
    return subStringTE->toPlainText();
}

void TodoBlock::connectSignalSlot()
{

}


