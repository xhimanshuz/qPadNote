#include "settingdialog.h"
#include<QDebug>

SettingDialog::SettingDialog(int _fontSize, QWidget *parent): QDialog(parent)
{
    fontSize = _fontSize;
    QTabWidget *fontTab = new QTabWidget(this);
    QWidget *fontWidget = new QWidget;

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(new QLabel("Font Size"));
    spin = new QSpinBox;
    spin->setValue(fontSize);
    hbox->addWidget(spin);
    fontWidget->setLayout(hbox);
    fontTab->addTab(fontWidget, tr("Font"));

    ok = new QPushButton("OK");
    connect(ok, SIGNAL(clicked()), this, SLOT(okClicked()));
    cancel = new QPushButton("Cancel");
    connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
    QHBoxLayout *hBox = new QHBoxLayout;
    hBox->addStretch();
    hBox->addWidget(ok);
    hBox->addWidget(cancel);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(fontTab);
    vbox->addLayout(hBox);

    setLayout(vbox);
}


void SettingDialog::okClicked()
{
    fontSize = spin->value();
    this->accept();
}
