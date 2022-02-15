#ifndef FIREBASE_H
#define FIREBASE_H

#include <firebaseauth.h>
#include <stdlib.h>

#include <QJsonObject>
#include <QVariant>

#include "AbstractIO.h"
#include "firebase/analytics.h"
#include "firebase/app.h"
#include "firebase/auth.h"
#include "firebase/database.h"
#include "firebase/variant.h"
#include "firebase/database/database_reference.h"

class DataEngine;
class ApplicationConfig;

// TODO: Remove Raw pointers
class Firebase : public AbstractIO {
  firebase::App *app;
  firebase::database::Database *db;
  firebase::database::DatabaseReference blockRef;

  DataEngine *dataEngine;
  std::string username;
  ApplicationConfig *appConfig;
  FirebaseAuth *auth;

public:
  Firebase();
  ~Firebase();
  // TODO: Cahnge the writeData name to hashSync
  void writeData() override;
  void writeBlock(TodoBlock &todoBlock) override;
  void writeBlocks(std::vector<TodoBlock *> &blocksVector) override;
  void removeBlock(int64_t _id) override;

  void getBlocks(int32_t uid) override;
  void getBlocksByTid(int32_t uid, std::string tid) override;

  void receiveBlocks(uint16_t size, uint8_t quantity) override;

  void removeTab(std::string tid, uint32_t _uid = 0) override;
  void renameTab(std::string xtid, std::string tid) override;
  void addTab(const std::string &tid) override;

  //    void removeTab(std::string tid);
  void removeBlock(int64_t id, const std::string todo,
                   const std::string &tid) override;
  void fetchAll() override;
  void hashModified() override;
  void moveBlock(std::string xid, bool toogle, std::string tid,
                 std::string id) override;
  inline const firebase::database::DataSnapshot*
  async_getSnapshot(firebase::Future<firebase::database::DataSnapshot> snapshot);

  inline const firebase::database::DataSnapshot*
  getSnapshot(firebase::Future<firebase::database::DataSnapshot> snapshot);


  static Firebase *getInstance();
  static Firebase *instance;

  bool authenticate();
  bool doSignIn(const QString &email, const QString &password);
  bool doSignUp(const QString &email, const QString &password);
  void loginSuccessed(firebase::auth::User *user);
  void loginSuccessed(firebase::auth::User *user, QString const& password);

};

#endif // FIREBASE_H
