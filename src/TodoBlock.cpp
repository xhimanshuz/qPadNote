#include "TodoBlock.h"
#include "dataEngine.h"

TodoBlock::TodoBlock(int64_t _id, std::string _tid, std::string _title, std::string _subString, uint32_t _hash, bool _toDone, QWidget *parent) : QWidget(parent),
    id{_id}, tid{_tid}, title(_title), subString{_subString}, hash{makeHash()}, isToDone{_toDone}, uid{1000}, position{0}, showSub{true}
{
//    dataEngine = DataEngine::getInstance();
    this->setParent(parent);
    if( _hash != hash )
        std::cout<<"[!] Hash not match old hash: "<< _hash<< " new Hash: "<<hash <<std::endl;
    mainLayout = new QVBoxLayout;
    renderUi();
    setLayout(mainLayout);
}

void TodoBlock::renderUi()
{
//    auto dataEngine = DataEngine::getInstance();
    titleCheckbox = new QCheckBox(title.c_str());
    titleCheckbox->setToolTip(getStatusTip());
//    titleCheckbox->setFont(QFont(QString(dataEngine->config.fontFamily.c_str()), dataEngine->config.fontSize, QFont::Bold));

    titleCheckbox->setChecked(isToDone);
    connect(titleCheckbox, &QCheckBox::toggled, [=](bool toggle){
        emit moveBlock(toggle, id);
    });

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

uint32_t TodoBlock::makeHash()
{
    std::stringstream ss;
    ss << id << tid << title << subString << isToDone << tid << uid;
    return strHash(ss.str());
}

bool TodoBlock::isHashModified()
{
    return hash != makeHash();
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

    time_t rawtime = id;
    char ct[10];
    char cd[25];
    time (&rawtime);
    auto timeinfo = std::localtime(&rawtime);

    std::strftime(ct, 100, "%I:%M %p", timeinfo);
    std::strftime(cd, 100, "%a, %m/%d/%y", timeinfo);

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

