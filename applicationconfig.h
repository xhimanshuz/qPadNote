#ifndef APPLICATIONCONFIG_H
#define APPLICATIONCONFIG_H

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <chrono>
#include <iostream>

namespace firebase::auth {
class User;
}

enum class UserType : char {
  ANONYMOUS = 'A',
  GOOGLE = 'G',
  FACEBOOK = 'F',
  MANUAL = 'M'
};

class ApplicationConfig {
  UserType _type;
  QString _email;
  QString _password;
  bool _loggedIn;
  QString _filename;
  firebase::auth::User *_user;

public:
  ApplicationConfig();
  ~ApplicationConfig();

  bool loadSetting(const QString &filename);
  bool writeSetting(const QString email, UserType type, QString const& password = "");

  UserType getUserType() const;
  QString getEmail() const;
  bool isLoggedIn() const;
  firebase::auth::User *getUser() const;
  void getUserInfo();

  void setUserType(UserType userType);
  void setEmail(const QString &email);
  void setLoggedIn(bool status);
  void setUser(firebase::auth::User *user);
  void setManualUser(firebase::auth::User *user, QString const &password);
  void writeUser();

  static ApplicationConfig *getInstance();
  static ApplicationConfig *instance;
  void setPassword(const QString &newPassword);
  const QString &getPassword() const;
};

#endif // APPLICATIONCONFIG_H
