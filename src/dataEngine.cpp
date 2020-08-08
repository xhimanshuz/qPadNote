#include "dataEngine.h"
#include <QDebug>

DataEngine* DataEngine::instance = nullptr;

DataEngine::DataEngine(): fileName{"config.json"}
{
    tabBlockMap = std::make_shared<std::map<std::string, std::pair< std::shared_ptr<std::map<std::string, TodoBlock*>>, std::shared_ptr<std::map<std::string, TodoBlock*>> >>>();
//    networkEngine = new NetworkEngine;
    config.fontSize = 13;
    config.fontFamily = "Roboto";

    jsonToMap(readData());
}

DataEngine::~DataEngine()
{

}


DataEngine* DataEngine::getInstance()
{
    if(!instance)
        instance = new DataEngine();
    return instance;
}

QJsonDocument DataEngine::mapToJson()
{
    QJsonObject mainJ;

    QJsonObject userDataJ;
    for(auto tab: *tabBlockMap)
    {
        QJsonObject tabJ;

        QJsonObject todoJ;
        for(auto todo: *tab.second.first)
        {
            QJsonObject todoBlock;
            todoBlock.insert("id", todo.second->id.c_str());
            todoBlock.insert("position", todo.second->position);
            todoBlock.insert("subString", todo.second->subString.c_str());
            todoBlock.insert("tabName", todo.second->tid.c_str());
            todoBlock.insert("title", todo.second->title.c_str());
            todoBlock.insert("type", todo.second->isToDone);
            todoBlock.insert("hash", todo.second->hash.c_str());
            todoJ.insert(todo.second->id.c_str(), todoBlock);
        }
        tabJ.insert("todo", todoJ);

        QJsonObject toDoneJ;
        for(auto toDone: *tab.second.second)
        {
            QJsonObject toDoneBlock;
            toDoneBlock.insert("id", toDone.second->id.c_str());
            toDoneBlock.insert("position", toDone.second->position);
            toDoneBlock.insert("subString", toDone.second->subString.c_str());
            toDoneBlock.insert("tabName", toDone.second->tid.c_str());
            toDoneBlock.insert("title", toDone.second->title.c_str());
            toDoneBlock.insert("type", toDone.second->isToDone);
            toDoneBlock.insert("hash", toDone.second->hash.c_str());
            toDoneJ.insert(toDone.second->id.c_str(), toDoneBlock);
        }
        tabJ.insert("toDone", toDoneJ);

        userDataJ.insert(tab.first.c_str(), tabJ);
    }
    mainJ.insert("userData", userDataJ);

    QJsonObject configJ;
    configJ.insert("fontSize", config.fontSize);
    configJ.insert("fontFamily", config.fontFamily.c_str());

    mainJ.insert("appData", configJ);


    return QJsonDocument(mainJ);
}


void DataEngine::jsonToMap(QJsonObject jObj)
{
    auto tabJ = jObj.value("userData").toObject();
    for(auto t=tabJ.begin(); t!=tabJ.end(); t++)
    {
        auto tabName = t.key().toStdString();
        auto todoJ = t.value().toObject().value("todo").toObject();
        auto toDoneJ = t.value().toObject().value("toDone").toObject();

        auto todoBlockMap = std::make_shared<std::map<std::string, TodoBlock*>>();
        for(auto todo = todoJ.begin(); todo!=todoJ.end(); ++todo)
        {
            auto id = todo.key().toLong();
            auto position = todo->toObject().value("position").toString().toStdString();
            auto subString = todo->toObject().value("subString").toString().toStdString();
            auto tabName = todo->toObject().value("tabName").toString().toStdString();
            auto title = todo->toObject().value("title").toString().toStdString();
            auto type = todo->toObject().value("type").toBool();
            auto hash = todo->toObject().value("hash").toString().toStdString();

            todoBlockMap->insert(std::make_pair(std::to_string(id), new TodoBlock(std::to_string(id), tabName, title, subString, hash, type)));
        }

        auto doneBlockMap = std::make_shared<std::map<std::string, TodoBlock*>>();
        for(auto toDone = toDoneJ.begin(); toDone!=toDoneJ.end(); ++toDone)
        {
            int64_t id = toDone.key().toLong();
            auto position = toDone->toObject().value("position").toString().toStdString();
            auto subString = toDone->toObject().value("subString").toString().toStdString();
            auto tabName = toDone->toObject().value("tabName").toString().toStdString();
            auto title = toDone->toObject().value("title").toString().toStdString();
            auto type = toDone->toObject().value("type").toBool();
            auto hash = toDone->toObject().value("hash").toString().toStdString();

            doneBlockMap->insert(std::make_pair(std::to_string(id), new TodoBlock(std::to_string(id), tabName, title, subString, hash, type)));
        }

        tabBlockMap->insert(std::make_pair(tabName, std::make_pair(todoBlockMap, doneBlockMap)));

    }

    auto appJ = jObj.value("appData").toObject();
    if(appJ.size())
    {
        config.fontSize = appJ.value("fontSize").toInt();
        config.fontFamily = appJ.value("fontFamily").toString().toStdString();
    }
}

QJsonObject DataEngine::readData()
{
    QFile file(fileName.c_str());
    if(!file.open(QFile::ReadOnly))
    {
        qDebug()<<" Error I/O Read File "<< fileName.c_str();
        writeData();
        return QJsonObject();
    }
    auto f = file.readAll();

    QJsonDocument jDoc = QJsonDocument::fromJson(f);
    file.close();
    return jDoc.object();
}

void DataEngine::writeData()
{
    QFile file(fileName.c_str());
    if(!file.open(QFile::WriteOnly))
    {
        qDebug()<<" Error I/O Writing File "<< fileName.c_str();
        return;
    }

    file.write(mapToJson().toJson());
    file.close();
}

void DataEngine::deleteBlock(std::string id, const std::string tabName)
{
   auto tMap = tabBlockMap->find(tabName);

   if(tMap->second.first->find(id) != tMap->second.first->end())
   {
       delete tabBlockMap->find(tabName)->second.first->find(id)->second;
       tMap->second.first->erase(id);
   }
   else
   {
       delete tabBlockMap->find(tabName)->second.second->find(id)->second;
       tMap->second.second->erase(id);
   }

    writeData();
}

void DataEngine::createTabMap(const std::string &tabName)
{
    tabBlockMap->insert(std::make_pair(tabName, std::make_pair(std::make_shared<std::map<std::string, TodoBlock*>>(), std::make_shared<std::map<std::string, TodoBlock*>>())));
}

void DataEngine::removeTabMap(const std::string &tabName)
{
    tabBlockMap->find(tabName)->second.first.reset();
    tabBlockMap->find(tabName)->second.second.reset();
    this->tabBlockMap->erase(tabName);
}

void DataEngine::renameTabMap(const std::string &oldName, const std::string &newName)
{

    tabBlockMap->insert(std::make_pair(newName, std::move(tabBlockMap->find(oldName)->second)));
    tabBlockMap->erase(oldName);
}
