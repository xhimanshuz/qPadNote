#include "userui.h"
#include "Firebase.h"
#include "applicationconfig.h"

UserUI::UserUI(QWidget *parent) : QDialog(parent) {
  mainLayout = new QVBoxLayout;
  appConfig = ApplicationConfig::getInstance();
  renderUi();
  setLayout(mainLayout);
  setWindowFlags(Qt::WindowType::FramelessWindowHint);
}

void UserUI::renderUi() {
  stackLaytout = new QStackedLayout;
  stackLaytout->addWidget(signInWidget());
  stackLaytout->addWidget(signUpWidget());
  if(appConfig->isLoggedIn()) {
    stackLaytout->addWidget(userDetail());
    stackLaytout->setCurrentIndex(2);
  }
  mainLayout->addLayout(stackLaytout);

  statusBar = new QStatusBar(this);
  mainLayout->addWidget(statusBar);

  signalAndSlot();
}

void UserUI::signalAndSlot() {
  connect(inSignUpPushButton, &QPushButton::clicked,
          [this] { stackLaytout->setCurrentIndex(1); });

  connect(inSignInPushButton, &QPushButton::clicked, [this] {
    stackLaytout->setCurrentIndex(0);
    auto email = signInEmail->text();
    auto password = signInPassword->text();
    if (!Firebase::getInstance()->doSignIn(email, password)) {
      statusBar->showMessage("Invlid Credentials or Some Error!", 0);
      return;
    }
    statusBar->showMessage("Login Successfully", 0);
  });
  connect(upSignInButton, &QPushButton::clicked,
          [this] { stackLaytout->setCurrentIndex(0); });
  connect(upSignUpButton, &QPushButton::clicked, [this] {
    stackLaytout->setCurrentIndex(1);
    auto email = signUpEmail->text();
    auto password = signUpPassword->text();
    auto password2 = signUpPassword2->text();
    if (password != password2) {
      statusBar->showMessage("Password didn't match", 0);
      return;
    }
    if (!Firebase::getInstance()->doSignUp(email, password)) {
      statusBar->showMessage("Invlid Credentials or Some Error!", 0);
      return;
    }

    statusBar->showMessage("Account created Successfully, Please Verify", 0);
  });
}

QWidget *UserUI::signUpWidget() {
  QLabel *heading = new QLabel;
  heading->setText("<h2>Sign in<h2>");

  signUpEmail = new QLineEdit;
  signUpPassword = new QLineEdit;
  signUpPassword->setEchoMode(QLineEdit::EchoMode::Password);
  signUpPassword2 = new QLineEdit;
  signUpPassword2->setEchoMode(QLineEdit::EchoMode::Password);
  upSignUpButton = new QPushButton("Sign up");
  upSignInButton = new QPushButton("Sign in");

  QFormLayout *form = new QFormLayout;
  form->addWidget(heading);
  form->addRow(new QLabel("EMail"), signUpEmail);
  form->addRow(new QLabel("Password"), signUpPassword);
  form->addRow(new QLabel("Re-Password"), signUpPassword2);
  QHBoxLayout *buttonBox = new QHBoxLayout;
  buttonBox->addStretch();
  buttonBox->addWidget(upSignUpButton);
  buttonBox->addWidget(upSignInButton);
  form->addItem(buttonBox);

  QWidget *_signUpWidget = new QWidget;
  _signUpWidget->setLayout(form);
  return _signUpWidget;
}

QWidget *UserUI::signInWidget() {
  QLabel *heading = new QLabel;
  heading->setText("<h2>Sign in<h2>");
  signInEmail = new QLineEdit;
  signInPassword = new QLineEdit;
  signInPassword->setEchoMode(QLineEdit::EchoMode::Password);
  inSignInPushButton = new QPushButton("Sign in");
  inSignUpPushButton = new QPushButton("Sign up");

  QFormLayout *form = new QFormLayout;
  form->addWidget(heading);
  form->addRow(new QLabel("Email"), signInEmail);
  form->addRow(new QLabel("Password"), signInPassword);
  QHBoxLayout *buttonBox = new QHBoxLayout;
  buttonBox->addStretch();
  buttonBox->addWidget(inSignInPushButton);
  buttonBox->addWidget(inSignUpPushButton);
  form->addItem(buttonBox);

  QWidget *_signInWidget = new QWidget;
  _signInWidget->setLayout(form);
  return _signInWidget;
}

QWidget *UserUI::userDetail() {
  QLabel *heading = new QLabel("<h2>`User Detail<h2>", this);
  QFormLayout *form = new QFormLayout(this);
  form->addWidget(heading);
  form->addRow("Email", new QLabel(appConfig->getEmail()));
  form->addRow("UID", new QLabel(appConfig->getUser()->uid().c_str()));
  inLogoutPushButton = new QPushButton("Logout");
  QHBoxLayout *buttonBox = new QHBoxLayout;
  buttonBox->addStretch();
  buttonBox->addWidget(inLogoutPushButton);
  form->addItem(buttonBox);

  QWidget* userDetailWidget = new QWidget(this);
  userDetailWidget->setLayout(form);
  return userDetailWidget;
}
