#include "Firebase.h"
#include "dataEngine.h"

Firebase* Firebase::instance = nullptr;

// TODO: blockRef in single line like data/user/app_data..
Firebase::Firebase()
{
    dataEngine = DataEngine::getInstance();
    username = "manik3112";
    auto option = firebase::AppOptions();
    option.set_database_url("https://qpadnote-id-default-rtdb.firebaseio.com");
    app = firebase::App::Create(option);

    firebase::analytics::Initialize(*app);
    firebase::analytics::SetAnalyticsCollectionEnabled(true);
    firebase::analytics::SetUserId(username.c_str());
    firebase::analytics::LogEvent("Manik3112 Started");
    firebase::analytics::LogEvent("name", "p_name","p_value");

    db = firebase::database::Database::GetInstance(app);
    db->set_persistence_enabled(true);
    blockRef = db->GetReference("data").Child(username).Child("app_data").Child("blocks");
    std::string block = blockRef.key_string();
    assert("blocks" == blockRef.key_string());

    fetchAll();
}

Firebase::~Firebase()
{
    firebase::analytics::Terminate();
}

void Firebase::writeData()
{
    hashModified();
}

void Firebase::writeBlock(TodoBlock &todoBlock)
{
    const std::string id{std::to_string(todoBlock.id)};
    std::map<std::string, firebase::Variant> block;
    block["id"] = id;
    block["position"] = static_cast<int64_t>(todoBlock.position);
    block["subString"] = todoBlock.subString;
    block["tid"] = todoBlock.tid;
    block["title"] = todoBlock.title;
    block["isToDone"] = todoBlock.isToDone;
    block["hash"] = static_cast<int64_t>(todoBlock.hash);
    block["uid"] = std::to_string(todoBlock.uid);

    std::cout << todoBlock.toString();
    blockRef.Child(todoBlock.tid).Child((todoBlock.isToDone)?"toDone":"todo").Child(id).SetValue(block);
}

void Firebase::writeBlocks(std::vector<TodoBlock *> &blocksVector)
{
    for(auto block: blocksVector)
        writeBlock(*block);
}

bool Firebase::writeHeader(Protocol::TYPE _type, uint16_t _bodySize, uint8_t _quantity)
{

}

void Firebase::removeBlock(int64_t _id)
{

}

void Firebase::getBlocks(int32_t uid)
{

}

void Firebase::getBlocksByTid(int32_t uid, std::string tid)
{

}

void Firebase::receiveBlocks(uint16_t size, uint8_t quantity)
{

}

void Firebase::removeTab(std::string tid, uint32_t _uid)
{
    blockRef.Child(tid).RemoveValue();
}



void Firebase::renameTab(std::string xtid, std::string tid)
{
    if(!blockRef.Child(xtid).is_valid())
    {
        std::cerr << "[x] Error in renaming tab, tab not found";
        return;
    }

    auto tab = blockRef.Child(xtid).GetValue();
    if(tab.error())
    {
       std::cout << "[x] Error while renaming"<< tab.error_message()<<std::endl;
       return;
    }
    while(tab.status() == firebase::FutureStatus::kFutureStatusPending);

    blockRef.Child(tid).SetValue(tab.result()->value());
    blockRef.Child(xtid).RemoveValue();
}

void Firebase::addTab(const std::string &tid)
{
    blockRef.Child(tid).SetValue(0);
}


void Firebase::removeBlock(int64_t id, const std::string todo, const std::string &tid)
{
    blockRef.Child(tid).Child(todo).Child(std::to_string(id)).RemoveValue();
}

//TODO: check hashVector
void Firebase::fetchAll()
{
    std::vector<uint32_t> hashVector;
//    int16_t uid{0};
    auto blockSnapshot = blockRef.GetValue();
    auto snapshot = getSnapshot(blockSnapshot);
    if(!snapshot)
        return;

    for(auto& tab: snapshot.value()->children())
    {
        auto tabName = tab.key_string();
        auto todoBlockMap = std::make_shared<std::map<int64_t, TodoBlock*>>();
        auto doneBlockMap = std::make_shared<std::map<int64_t, TodoBlock*>>();
        for(auto& toDone: tab.children())
        {
            for(auto& block: toDone.children())
            {

                auto blockMap = block.value().map();
                auto id = std::atol(block.key_string().c_str());
                auto position = blockMap["position"].int64_value();
                auto subString = blockMap["subString"].string_value();
                auto tid = blockMap["tid"].string_value();
                auto title = blockMap["title"].string_value();
                auto isToDone = blockMap["isToDone"].bool_value();
                auto hash = blockMap["hash"].int64_value();
                auto uid = blockMap["uid"].string_value();

                ((isToDone)?doneBlockMap:todoBlockMap)->insert({id, new TodoBlock(id, tabName, title, subString, hash, isToDone)});
            }
        }
        dataEngine->tabBlockMap->insert(std::make_pair(tabName, std::make_pair(todoBlockMap, doneBlockMap)));
    }

}

void Firebase::hashModified()
{
    std::cout<<"[!] HashModified!"<<std::endl;
    std::vector<TodoBlock*> modifiedVecter;
    for(auto tab: *dataEngine->tabBlockMap)
    {
        for(auto block: *tab.second.first)
        {
            if(block.second->isHashModified())
            {
                block.second->hash = block.second->makeHash();
                modifiedVecter.push_back(block.second);
            }
        }

        for(auto block: *tab.second.second)
        {
            if(block.second->isHashModified())
            {
                block.second->hash = block.second->makeHash();
                modifiedVecter.push_back(block.second);
            }
        }
    }
    if(!modifiedVecter.empty())
        writeBlocks(modifiedVecter);

}

void Firebase::moveBlock(std::string xid, bool toogle, std::string tid, std::string id)
{
    auto blockPromise = blockRef.Child(tid).Child((toogle)?"todo":"toDone").Child(xid).GetValue();
    auto block = getSnapshot(blockPromise);
    if(!block)
    {
        std::cerr << "[x] Error in moveing block "<< xid << " tid: "<< tid;
        return;
    }

    blockRef.Child(tid).Child((!toogle)?"todo":"toDone").Child(id).SetValue(block.value()->value());
    blockRef.Child(tid).Child((toogle)?"todo":"toDone").Child(xid).RemoveValue();

}

inline std::optional<const firebase::database::DataSnapshot*> Firebase::getSnapshot(firebase::Future<firebase::database::DataSnapshot> snapshot)
{
    if(snapshot.error())
    {
       std::cout << "[x] Error while fecthing snapshot"<< snapshot.error_message()<<std::endl;
       return std::nullopt;
    }
    while(snapshot.status() == firebase::FutureStatus::kFutureStatusPending);
    return std::make_optional(snapshot.result());
}

Firebase *Firebase::getInstance()
{
    if(!instance)
        instance = new Firebase;
    return instance;
}
