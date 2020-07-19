#include "TodoBlock.h"
#include "dataEngine.h"

TodoBlock::TodoBlock(std::string _id, std::string _title, std::string _subString, bool _toDone, QWidget *parent) : QWidget(parent),
    showSub{true}, title(_title), id{_id}, subString{_subString}, isToDone{_toDone}, tid{"NULL"}, uid{1000}
{
//    dataEngine = DataEngine::getInstance();
    this->setParent(parent);
    mainLayout = new QVBoxLayout;
    renderUi();
    setLayout(mainLayout);
}

void TodoBlock::renderUi()
{
    auto dataEngine = DataEngine::getInstance();
    titleCheckbox = new QCheckBox(title.c_str());
    titleCheckbox->setToolTip(getStatusTip());
    titleCheckbox->setFont(QFont(QString(dataEngine->config.fontFamily.c_str()), dataEngine->config.fontSize, QFont::Bold));

    titleCheckbox->setChecked(isToDone);
    connect(titleCheckbox, &QCheckBox::toggled, [=](bool toggle){ moveBlock(toggle, id); });

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->setMargin(2);
    hbox->addWidget(titleCheckbox, 1, Qt::AlignmentFlag::AlignLeft);
    hbox->addStretch();

    showSubButton = new QAction;
    showSubButton->setIcon(QIcon("://moreV.png"));
    showSubButton->setCheckable(true);
    showSubButton->setToolTip("Show More");
//    showSubButton->set

    connect(showSubButton, &QAction::toggled, [this](bool toggle){
        if(!toggle)
        {
            showSubButton->setIcon(QIcon("://moreV.png"));
            subWidget->hide();
        }
        else
        {
            showSubButton->setIcon(QIcon("://moreH.png"));
            subWidget->show();
        }
    });
//    hbox->addWidget(dateLabel, 0, Qt::AlignmentFlag::AlignRight);
    deleteToolButton = new QAction;
    connect(deleteToolButton, &QAction::triggered, this, [=]{ emit deleteBlock(id); });
    deleteToolButton->setIcon(QIcon("://remove.png"));

    blockToolBar = new QToolBar;
    blockToolBar->addAction(showSubButton);
    blockToolBar->addAction(deleteToolButton);
    blockToolBar->setIconSize(QSize(15, 15));
    blockToolBar->setFloatable(false);
    blockToolBar->setMovable(true);
    blockToolBar->setBaseSize(QSize(0, 0));
    hbox->addWidget(blockToolBar);

    mainLayout->addLayout(hbox);
//    mainLayout->addWidget(showHideButton);

    mainLayout->addWidget(createMoreWidget());

    mainLayout->setMargin(0);

    connectSignalSlot();
}



QString TodoBlock::getStatusTip()
{
    return QString("<b>Task:</b> %0 \n<b>Summary:</b> %1 \n<b>Time:</b> %2").arg(title.c_str()).arg(subString.c_str()).arg("");

}

void TodoBlock::connectSignalSlot()
{

}

QWidget* TodoBlock::createMoreWidget()
{
    QVBoxLayout* subLayout = new QVBoxLayout();

    subStringTE = new QTextEdit(subString.c_str());
    connect(subStringTE, &QTextEdit::textChanged, [this]{
        subString = subStringTE->toPlainText().toStdString();
    });
//    subStringTE->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum));
    subStringTE->setFixedHeight(subStringTE->sizeHint().height()/2);

    subLayout->addWidget(subStringTE);

    const long time = atoll(std::string(id, 0, 10).c_str());
    char ct[10];
    char cd[25];
    std::strftime(ct, 100, "%H:%M %p", std::localtime(&time));
    std::strftime(cd, 100, "%a, %m/%d/%y", std::localtime(&time));

    QVBoxLayout *tVBox = new QVBoxLayout;
    tVBox->addWidget(new QLabel(tr("<B>%0</b>").arg(ct)), 1, Qt::AlignmentFlag::AlignRight);
    tVBox->addWidget(new QLabel(tr("%0").arg(cd)), 1, Qt::AlignmentFlag::AlignRight);
    tVBox->setSpacing(0);
//    subLayout->addWidget(dateLabel);

    QCheckBox *lockCheckBox = new QCheckBox("Lock");
    connect(lockCheckBox, &QCheckBox::toggled, subStringTE, &QTextEdit::setReadOnly);

//    subToolButton = new QToolButton;
//    subToolButton->setArrowType(Qt::ArrowType::DownArrow);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(lockCheckBox);
//    hbox->addWidget(subToolButton);
    hbox->addStretch();
    hbox->addLayout(tVBox);
    subLayout->addLayout(hbox);
    hbox->setMargin(0);

    subWidget = new QWidget;
    subWidget->setLayout(subLayout);
    subWidget->hide();

    return subWidget;
}

