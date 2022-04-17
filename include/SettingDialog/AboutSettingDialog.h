#ifndef ABOUTSETTINGDIALOG_H
#define ABOUTSETTINGDIALOG_H

#include <QWidget>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

class AboutSettingDialog : public QWidget
{
  Q_OBJECT

  QTextEdit creditList;
  QVBoxLayout mainLayout;
  QLabel github;

  void renderUI();
  void setSignalSlot();
 public:
  explicit AboutSettingDialog(QWidget *parent = nullptr);

 signals:
  void onOkClick();
  void onCancelClick();
  void onCheckUpdateClick();
};

#endif // ABOUTSETTINGDIALOG_H
