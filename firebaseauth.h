#ifndef FIREBASEAUTH_H
#define FIREBASEAUTH_H

#include "firebase/app.h"
#include "firebase/auth.h"
#include "firebase/auth/user.h"

#include "applicationconfig.h"

class FirebaseAuth {
  firebase::App *_app;
  firebase::auth::Auth *_auth;

public:
  FirebaseAuth(firebase::App *app);

  firebase::auth::User *signIn(const UserType type, const QString &email = "",
                               const QString &password = "");
  firebase::auth::User *signUp(const UserType type, const QString &email = "",
                               const QString &password = "");
  bool logout();

  template <typename T> T *syncFuture(firebase::Future<T *> future);
};

#endif // FIREBASEAUTH_H
