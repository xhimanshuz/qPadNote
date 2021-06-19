#include "firebaseauth.h"

#include <iostream>

FirebaseAuth::FirebaseAuth(firebase::App *app) : _app{app} {
  _auth = firebase::auth::Auth::GetAuth(_app);
}

firebase::auth::User *FirebaseAuth::signIn(const UserType type,
                                           const QString &email,
                                           const QString &password) {
  firebase::Future<firebase::auth::User *> userFuture;

  switch (type) {
  case UserType::ANONYMOUS:
    userFuture = _auth->SignInAnonymously();
    break;
  case UserType::MANUAL:
    userFuture = _auth->SignInWithEmailAndPassword(
        email.toStdString().c_str(), password.toStdString().c_str());
    break;
  default:
    break;
  }

  auto user = syncFuture<firebase::auth::User>(userFuture);
  return user;
}

firebase::auth::User *FirebaseAuth::signUp(const UserType type,
                                           const QString &email,
                                           const QString &password) {
  firebase::Future<firebase::auth::User *> userFuture;

  switch (type) {
  case UserType::ANONYMOUS: {
    userFuture = _auth->SignInAnonymously();
    break;
  }
  case UserType::MANUAL: {
    userFuture = _auth->CreateUserWithEmailAndPassword(
        email.toStdString().c_str(), password.toStdString().c_str());
    break;
  }
  default:
    break;
  }

  auto user = syncFuture<firebase::auth::User>(userFuture);

  return user;
}

template <typename T>
T *FirebaseAuth::syncFuture(firebase::Future<T *> future) {
  if (future.error()) {
    std::cerr << "[!] Error in SyncFuture: " << future.error_message()
              << std::endl;
    return nullptr;
  }
  while (future.status() == firebase::FutureStatus::kFutureStatusPending)
    ;
  return *future.result();
}
