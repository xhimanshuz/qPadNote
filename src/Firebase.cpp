#include "Firebase.h"
#include "applicationconfig.h"
#include "dataEngine.h"

Firebase *Firebase::instance = nullptr;

// TODO: blockRef in single line like data/user/app_data..
Firebase::Firebase() {
  dataEngine = DataEngine::getInstance();

  // Firebase App objection creation.
  auto option = firebase::AppOptions();
  option.set_database_url("https://qpadnote-id-default-rtdb.firebaseio.com");
  app = firebase::App::Create(option);

  // Firebase Auth object creation.
  auth = new FirebaseAuth(app);
  appConfig = ApplicationConfig::getInstance();
  if (!appConfig->loadSetting("qPadNote.conf"))
    qDebug() << "Error In loading Config";

  // Firebase Database object creation.
  db = firebase::database::Database::GetInstance(app);
  db->PurgeOutstandingWrites();
  //  db->set_persistence_enabled(true);

  if (authenticate()) {
    std::string block = blockRef.key_string();
    std::cout << "BlocksRef: " << block << std::endl;
    assert("blocks" == blockRef.key_string());
  }
}

Firebase::~Firebase() {}

void Firebase::writeData() { hashModified(); }

void Firebase::writeBlock(TodoBlock &todoBlock) {
  const std::string id{std::to_string(todoBlock.id)};
  std::map<std::string, firebase::Variant> block;
  block["id"] = id;
  block["position"] = static_cast<int64_t>(todoBlock.position);
  block["subString"] = todoBlock.subString;
  block["tid"] = todoBlock.tid;
  block["title"] = todoBlock.title;
  block["isToDone"] = (todoBlock.isToDone)?1:0;
  block["hash"] = static_cast<int64_t>(todoBlock.hash);
  block["uid"] = std::to_string(todoBlock.uid);

  std::cout << todoBlock.toString();
  blockRef.Child(todoBlock.tid)
      .Child((todoBlock.isToDone) ? "toDone" : "todo")
      .Child(id)
      .SetValue(block);
}

void Firebase::writeBlocks(std::vector<TodoBlock *> &blocksVector) {
  for (auto block : blocksVector)
    writeBlock(*block);
}

void Firebase::removeBlock(int64_t _id) {}

void Firebase::getBlocks(int32_t uid) {}

void Firebase::getBlocksByTid(int32_t uid, std::string tid) {}

void Firebase::receiveBlocks(uint16_t size, uint8_t quantity) {}

void Firebase::removeTab(std::string tid, uint32_t _uid) {
  blockRef.Child(tid).RemoveValue();
}

void Firebase::renameTab(std::string xtid, std::string tid) {
  if (!blockRef.Child(xtid).is_valid()) {
    std::cerr << "[x] Error in renaming tab, tab not found";
    return;
  }

  auto tab = blockRef.Child(xtid).GetValue();
  if (tab.error()) {
    std::cout << "[x] Error while renaming" << tab.error_message() << std::endl;
    return;
  }
  while (tab.status() == firebase::FutureStatus::kFutureStatusPending)
    ;

  blockRef.Child(tid).SetValue(tab.result()->value());
  blockRef.Child(xtid).RemoveValue();
}

void Firebase::addTab(const std::string &tid) {
  blockRef.Child(tid).SetValue(0);
}

void Firebase::removeBlock(int64_t id, const std::string todo,
                           const std::string &tid) {
  blockRef.Child(tid).Child(todo).Child(std::to_string(id)).RemoveValue();
}

//// TODO: check hashVector
void Firebase::fetchAll()
{
  std::vector<uint32_t> hashVector;
  int16_t uid{0};
  auto blockSnapshot = blockRef.GetValue();
  auto snapshot = getSnapshot(blockSnapshot);
  if(!snapshot)
    return;
  auto value = snapshot;

  QJsonObject json;
  for (auto &tab : value->children()) {
    json.insert(tab.key(), {});
    dataEngine->createTabMap(tab.key_string());
    QJsonObject todoJson;
    for(auto const td: {"todo", "toDone"}) {
      json[tab.key()].toObject().insert(td, {});
      if(tab.HasChild(td)) {
        QJsonObject blockJson;
        for(auto& block: tab.Child(td).children()) {
          std::cout << block.GetReference().url() << std::endl;
          blockJson.insert(block.key(),
                           QJsonObject {
                               {"id", block.Child("id").value().mutable_string().c_str()},
                               {"position",  QJsonValue::fromVariant(QVariant::fromValue(block.Child("position").value().int64_value()))},
                               {"subString", block.Child("subString").value().mutable_string().c_str()},
                               {"tabName", block.Child("tid").value().mutable_string().c_str()},
                               {"title", block.Child("title").value().mutable_string().c_str()},
                               {"type", QVariant::fromValue(block.Child("isToDone").value().int64_value()).toBool()},
                               {"uid", block.Child("uid").value().mutable_string().c_str()},
                               {"hash", QJsonValue::fromVariant(QVariant::fromValue(block.Child("hash").value().int64_value()))}
                           });
        }
        todoJson.insert(td, blockJson);
      }
    }
    json.insert(tab.key(), todoJson);
  }
  dataEngine->jsonToMap(QJsonObject{{"userData", json}});
}

void Firebase::hashModified()
{

}

void Firebase::moveBlock(std::string xid, bool toogle, std::string tid, std::string id)
{
  if(toogle) {
    auto from = getSnapshot(blockRef.Child(tid).Child("todo").Child(xid).GetValue());
    if(!from || !from->is_valid())
      return;
    auto value = from->value();
    blockRef.Child(tid).Child("toDone").Child(id).SetValue(value);
    blockRef.Child(tid).Child("toDone").Child(id).Child("isToDone").SetValue(1);
    blockRef.Child(tid).Child("todo").Child(xid).RemoveValue();
  }
  else {
    auto from = getSnapshot(blockRef.Child(tid).Child("toDone").Child(xid).GetValue());
    if(!from || !from->is_valid())
      return;
    auto value = from->value();
    blockRef.Child(tid).Child("todo").Child(id).SetValue(value);
    blockRef.Child(tid).Child("todo").Child(id).Child("isToDone").SetValue(0);
    blockRef.Child(tid).Child("toDone").Child(xid).RemoveValue();
  }
}

const firebase::database::DataSnapshot * Firebase::async_getSnapshot(firebase::Future<firebase::database::DataSnapshot> result)
{
  std::cout << "result status: " << result.status() << std::endl;
  if (result.status() != firebase::kFutureStatusPending) {
    if (result.status() != firebase::kFutureStatusComplete) {
      printf("ERROR: GetValue() returned an invalid result.\n");
      return nullptr;
      // Handle the error...
    } else if (result.error() != firebase::database::kErrorNone) {
      printf("ERROR: GetValue() returned error %d: %s\n", result.error(),
                 result.error_message());
      return nullptr;
      // Handle the error...
    }
  }
  const firebase::database::DataSnapshot* snapshot = result.result();
  return snapshot;
}

const firebase::database::DataSnapshot * Firebase::getSnapshot(firebase::Future<firebase::database::DataSnapshot> result)
{
  while(result.status() == firebase::kFutureStatusPending);

  std::cout << "result status: " << result.status() << std::endl;
  if (result.status() != firebase::kFutureStatusPending) {
    if (result.status() != firebase::kFutureStatusComplete) {
      printf("ERROR: GetValue() returned an invalid result.\n");
      return nullptr;
      // Handle the error...
    } else if (result.error() != firebase::database::kErrorNone) {
      printf("ERROR: GetValue() returned error %d: %s\n", result.error(),
             result.error_message());
      return nullptr;
      // Handle the error...
    }
  }
  const firebase::database::DataSnapshot* snapshot = result.result();
  return snapshot;
}

Firebase *Firebase::getInstance() {
  if (!instance)
    instance = new Firebase;
  return instance;
}

bool Firebase::authenticate() {
  if (appConfig->isLoggedIn()) {
    std::cout << "Alread Logged In\n";
  }

  auto email = appConfig->getEmail();
  auto type = appConfig->getUserType();
  auto password = appConfig->getPassword();

  auto user = auth->signIn(type, email, password);
  if (!user)
    return false;
  if(type == UserType::MANUAL)
    loginSuccessed(user, password);
  else
    loginSuccessed(user);
  return true;
}

bool Firebase::doSignIn(const QString &email, const QString &password) {
  auto user = auth->signIn(UserType::MANUAL, email, password);
  if (!user)
    return false;
  loginSuccessed(user, password);
  return true;
}

bool Firebase::doSignUp(const QString &email, const QString &password) {
  auto user = auth->signUp(UserType::MANUAL, email, password);
  if (!user)
    return false;
  loginSuccessed(user, password);
  return true;
}

void Firebase::loginSuccessed(firebase::auth::User *user) {
  appConfig->setUser(user);
  appConfig->writeUser();
  appConfig->getUserInfo();
  appConfig->setLoggedIn(true);

  username = (appConfig->getUser()->is_anonymous())
                 ? appConfig->getUser()->uid()
                 : appConfig->getUser()->email();

  std::cout << "[!] Current Username: " << username << std::endl;
  blockRef = db->GetReference("data")
                 .Child(user->uid())
                 .Child("app_data")
                 .Child("blocks").GetReference();

  std::string block = blockRef.key_string();
  std::cout << "BlocksRef: " << block << " " << blockRef.url() << " " << blockRef.is_valid() << std::endl;
  assert("blocks" == blockRef.key_string());

  fetchAll();
}

void Firebase::loginSuccessed(firebase::auth::User *user, const QString &password)
{
  appConfig->setManualUser(user, password);
  appConfig->writeUser();
  appConfig->getUserInfo();
  appConfig->setLoggedIn(true);

  username = (appConfig->getUser()->is_anonymous())
                 ? appConfig->getUser()->uid()
                 : appConfig->getUser()->email();

  std::cout << "[!] Current Username: " << username << std::endl;
  blockRef = db->GetReference("data")
                 .Child(user->uid())
                 .Child("app_data")
                 .Child("blocks").GetReference();

  std::string block = blockRef.key_string();
  std::cout << "BlocksRef: " << block << " " << blockRef.url() << " " << blockRef.is_valid() << std::endl;
  assert("blocks" == blockRef.key_string());

  fetchAll();
}
