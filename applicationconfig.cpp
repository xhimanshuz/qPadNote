#include "applicationconfig.h"
ApplicationConfig *ApplicationConfig::instance = nullptr;

#include <QDebug>
#include <firebase/auth/user.h>

void ApplicationConfig::setPassword(const QString &newPassword)
{
  _password = newPassword;
}

const QString &ApplicationConfig::getPassword() const
{
  return _password;
}

ApplicationConfig::ApplicationConfig()
    : _type{UserType::ANONYMOUS}, _email{""}, _password{""}, _loggedIn{false}, _filename{""},
      _user{nullptr} {}

bool ApplicationConfig::loadSetting(const QString &filename) {
  _filename = filename;
  QFile file(_filename);
  if (!file.exists())
    writeSetting(
        QString::number(
            std::chrono::system_clock::now().time_since_epoch().count()),
        UserType::ANONYMOUS);

  if (!file.open(QFile::ReadOnly)) {
    qDebug() << file.errorString();
    return false;
  }

  QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
  QJsonObject json = doc.object();

  _email = json["email"].toString();
  _type = static_cast<UserType>(json["type"].toString().toStdString()[0]);
  _password = json["password"].toString();

  file.close();
  return true;
}

bool ApplicationConfig::writeSetting(const QString email, UserType type, QString const& password) {
  QFile file(_filename);
  if (!file.open(QFile::WriteOnly)) {
    qDebug() << file.errorString();
    return false;
  }
  QJsonObject json{{"email", email}, {"type", QString((char)type)}, {"password", password}};
  file.write(QJsonDocument(json).toJson());
  file.close();
  _email = email;
  _type = type;

  return true;
}

UserType ApplicationConfig::getUserType() const { return _type; }

QString ApplicationConfig::getEmail() const { return _email; }

bool ApplicationConfig::isLoggedIn() const { return _loggedIn; }

firebase::auth::User *ApplicationConfig::getUser() const { return _user; }

void ApplicationConfig::getUserInfo() {
  if (!_user) {
    std::cerr << "[x] User not found!\n";
    return;
  }

  std::cout << "[!] User Info\n"
            << "> Email: " << _user->email() << "\n"
            << "> UID: " << _user->uid() << "\n"
            << "> Provider_id: " << _user->provider_id() << "\n"
            << "> Phone Number: " << _user->phone_number() << "\n"
            << "> Email Verified: " << _user->is_email_verified() << "\n"
            << "> Anonymouse: " << _user->is_anonymous() << "\n";
}

void ApplicationConfig::setUserType(UserType userType) { _type = userType; }

void ApplicationConfig::setEmail(const QString &email) { _email = email; }

void ApplicationConfig::setLoggedIn(bool status) { _loggedIn = status; }

void ApplicationConfig::setUser(firebase::auth::User *user) { _user = user; }

void ApplicationConfig::setManualUser(firebase::auth::User *user, const QString &password) {
  setUser(user);
  setPassword(password);
}

void ApplicationConfig::writeUser() {
  if (!_user) {
    std::cerr << "[x] User not found!\n";
    return;
  }
  auto email = (_user->is_anonymous()) ? _user->uid() : _user->email();
  if (_user->provider_id() == "Firebase") {
    _type = (_user->is_anonymous()) ? UserType::ANONYMOUS : UserType::MANUAL;
  } else if (_user->provider_id() == "Facebook") {
    _type = UserType::FACEBOOK;
  }

  writeSetting(QString(email.c_str()), _type, _password);
}

ApplicationConfig *ApplicationConfig::getInstance() {
  if (!instance)
    instance = new ApplicationConfig;
  return instance;
}
