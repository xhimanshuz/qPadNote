#include "SettingDialog/SettingDialog.h"

SettingDialog::SettingDialog(QWidget* parent)
    : QDialog(parent)
//    , updateButton("Check Update", this)
    , okButton("OK", this)
//    , cancelButton("Cancel", this)
{
  mainLayout = new QVBoxLayout(this);
  renderUI();
  setLayout(mainLayout);
}

void SettingDialog::onOkClick()
{
  close();
}

void SettingDialog::onCancelClick()
{
  close();
}

void SettingDialog::onCheckUpdateClick()
{

}

void SettingDialog::renderUI()
{
  tabWidget.addTab(&aboutSettingDialog, "About");
  mainLayout->addWidget(&tabWidget);
  mainLayout->addStretch();

  QHBoxLayout *hbox = new QHBoxLayout(this);
  hbox->addStretch(2);
//  hbox->addWidget(&updateButton, 1);
  hbox->addWidget(&okButton, 1);
//  hbox->addWidget(&cancelButton, 1);
  mainLayout->addLayout(hbox, 1);

  setupSignalSlot();
}

void SettingDialog::setupSignalSlot()
{
  connect(&okButton, &QPushButton::clicked, this, &SettingDialog::onOkClick);
  connect(&cancelButton, &QPushButton::clicked, this, &SettingDialog::onCancelClick);
  connect(&updateButton, &QPushButton::clicked, this, &SettingDialog::onCheckUpdateClick);
}
