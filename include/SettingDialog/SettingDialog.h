#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

#include "SettingDialog/AboutSettingDialog.h"

class SettingDialog : public QDialog
{
  Q_OBJECT

  QVBoxLayout *mainLayout;
  QTabWidget tabWidget;
  QPushButton updateButton;
  QPushButton okButton;
  QPushButton cancelButton;

  AboutSettingDialog aboutSettingDialog;

  void renderUI();
  void setupSignalSlot();
 public:
  SettingDialog(QWidget* parent = nullptr);

 public slots:
  void onOkClick();
  void onCancelClick();
  void onCheckUpdateClick();

};

#endif // SETTINGDIALOG_H
