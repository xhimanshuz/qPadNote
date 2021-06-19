#ifndef USERUI_H
#define USERUI_H

#include <QDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedLayout>
#include <QStatusBar>
#include <QVBoxLayout>

class UserUI : public QDialog {
  Q_OBJECT
public:
  UserUI(QWidget *parent = nullptr);

  QVBoxLayout *mainLayout;
  QStackedLayout *stackLaytout;
  QLineEdit *signInEmail;
  QLineEdit *signInPassword;
  QLineEdit *signUpEmail;
  QLineEdit *signUpPassword;
  QLineEdit *signUpPassword2;
  QPushButton *inSignInPushButton;
  QPushButton *inSignUpPushButton;
  QPushButton *inLogoutPushButton;
  QPushButton *upSignUpButton;
  QPushButton *upSignInButton;
  QStatusBar *statusBar;

  void renderUi();
  void signalAndSlot();

  QWidget *signUpWidget();
  QWidget *signInWidget();
  QWidget *userDetail();
};

#endif // USERUI_H
