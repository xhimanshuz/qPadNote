#include "SettingDialog/AboutSettingDialog.h"


AboutSettingDialog::AboutSettingDialog(QWidget *parent)
    : QWidget(parent)
    , creditList(parent)
    , mainLayout(this)
      , github("Github: <a href=\"https://github.com/xhimanshuz/qPadNote\">https://github.com/xhimanshuz/qPadNote</a>", this)
{
  renderUI();
  setLayout(&mainLayout);
}

void AboutSettingDialog::renderUI()
{
  mainLayout.addWidget(new QLabel("<H1>Credits</H1>"));
  creditList.setPlainText("Core Developer:\n---------------\n"
      "Himanshu Rastogi <hi.himanshu14@gmail.com>");

  github.setOpenExternalLinks(true);

  mainLayout.addWidget(&creditList);
  mainLayout.addWidget(&github);
}

void AboutSettingDialog::setSignalSlot()
{
}
