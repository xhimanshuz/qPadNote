#include "dataEngine.h"
#include <QDebug>

std::shared_ptr<DataEngine> DataEngine::instance = nullptr;

DataEngine::DataEngine(): fileName{"sampleConfig.json"}
{
    tabMap = std::make_shared<std::map<std::string, std::pair<std::shared_ptr<std::map<std::string, std::array<std::string, 6> >>, std::shared_ptr<std::map<std::string, std::array<std::string, 6> >>>>>();
    tabBlockMap = std::make_shared<std::map<std::string, std::pair< std::shared_ptr<std::map<std::string, TodoBlock*>>, std::shared_ptr<std::map<std::string, TodoBlock*>> >>>();
//    networkEngine = new NetworkEngine;
    config.fontSize = 13;
    config.fontFamily = "Roboto";

    jsonToMap(readData());
}

DataEngine::~DataEngine()
{

}


std::shared_ptr<DataEngine> DataEngine::getInstance()
{
    if(!instance)
        instance = std::make_shared<DataEngine>();
    return instance;
}

QJsonDocument DataEngine::mapToJson()
{
    QJsonObject mainJ;

    QJsonObject userDataJ;
    for(auto tab: *tabMap)
    {
        QJsonObject tabJ;

        QJsonObject todoJ;
        for(auto todo: *tab.second.first)
        {
            QJsonObject todoBlock;
            todoBlock.insert("id", todo.second.at(0).c_str());
            todoBlock.insert("position", todo.second.at(1).c_str());
            todoBlock.insert("subString", todo.second.at(2).c_str());
            todoBlock.insert("tabName", todo.second.at(3).c_str());
            todoBlock.insert("title", todo.second.at(4).c_str());
            todoBlock.insert("type", todo.second.at(5).c_str());
            todoJ.insert(todo.second.at(0).c_str(), todoBlock);
        }
        tabJ.insert("todo", todoJ);

        QJsonObject toDoneJ;
        for(auto toDone: *tab.second.second)
        {
            QJsonObject toDoneBlock;
            toDoneBlock.insert("id", toDone.second.at(0).c_str());
            toDoneBlock.insert("position", toDone.second.at(1).c_str());
            toDoneBlock.insert("subString", toDone.second.at(2).c_str());
            toDoneBlock.insert("tabName", toDone.second.at(3).c_str());
            toDoneBlock.insert("title", toDone.second.at(4).c_str());
            toDoneBlock.insert("type", toDone.second.at(5).c_str());
            toDoneJ.insert(toDone.second.at(0).c_str(), toDoneBlock);
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

void DataEngine::syncMapUI()
{
    for(auto tab: *tabBlockMap)
    {
        auto tabM = tabMap->find(tab.first);
        auto todoBlock = tab.second.first;
        auto todoM = tabM->second.first;

        for(auto todoB: *todoBlock)
        {
            todoM->find(todoB.first)->second.at(0) = todoB.second->id;
            todoM->find(todoB.first)->second.at(1) = "0";
            todoM->find(todoB.first)->second.at(2) = todoB.second->subString;
            todoM->find(todoB.first)->second.at(3) = tabM->first;
            todoM->find(todoB.first)->second.at(4) = todoB.second->title;
            todoM->find(todoB.first)->second.at(5) = "0";
        }

        auto doneBlock = tab.second.second;
        auto doneM = tabM->second.second;

        for(auto doneB: *doneBlock)
        {
            doneM->find(doneB.first)->second.at(0) = doneB.second->id;
            doneM->find(doneB.first)->second.at(1) = "0";
            doneM->find(doneB.first)->second.at(2) = doneB.second->subString;
            doneM->find(doneB.first)->second.at(3) = tabM->first;
            doneM->find(doneB.first)->second.at(4) = doneB.second->title;
            doneM->find(doneB.first)->second.at(5) = "1";
        }

    }
}

void DataEngine::jsonToMap(QJsonObject jObj)
{
    auto tabJ = jObj.value("userData").toObject();
    for(auto t=tabJ.begin(); t!=tabJ.end(); t++)
    {
        auto tn = t.key().toStdString();
        auto todoJ = t.value().toObject().value("todo").toObject();
        auto toDoneJ = t.value().toObject().value("toDone").toObject();

        auto todoMap = std::make_shared<std::map<std::string, std::array<std::string, 6>>>();
        for(auto todo = todoJ.begin(); todo!=todoJ.end(); ++todo)
        {
            auto id = todo.key().toStdString();
            auto position = todo->toObject().value("position").toString().toStdString();
            auto subString = todo->toObject().value("subString").toString().toStdString();
            auto tabName = todo->toObject().value("tabName").toString().toStdString();
            auto title = todo->toObject().value("title").toString().toStdString();
            auto type = todo->toObject().value("type").toString().toStdString();
            todoMap->insert(std::pair<std::string, std::array<std::string, 6>>(id, {id, position, subString, tabName, title, type}));
        }

        auto toDoneMap = std::make_shared<std::map<std::string, std::array<std::string, 6>>>();
        for(auto toDone = toDoneJ.begin(); toDone!=toDoneJ.end(); ++toDone)
        {
            auto id = toDone.key().toStdString();
            auto position = toDone->toObject().value("position").toString().toStdString();
            auto subString = toDone->toObject().value("subString").toString().toStdString();
            auto tabName = toDone->toObject().value("tabName").toString().toStdString();
            auto title = toDone->toObject().value("title").toString().toStdString();
            auto type = toDone->toObject().value("type").toString().toStdString();
            toDoneMap->insert(std::pair<std::string, std::array<std::string, 6>>(id, {id, position, subString, tabName, title, type}));
        }

        tabMap->insert(std::make_pair(tn, std::make_pair(todoMap, toDoneMap)));
        tabBlockMap->insert(std::make_pair(tn, std::make_pair(std::make_shared<std::map<std::string, TodoBlock*>>(), std::make_shared<std::map<std::string, TodoBlock*>>())));
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

    syncMapUI();
    file.write(mapToJson().toJson());
    file.close();
}

void DataEngine::deleteBlock(std::string id, const std::string tabName)
{
   auto tMap = tabMap->find(tabName);

   if(tMap->second.first->find(id) != tMap->second.first->end())
   {
       tMap->second.first->erase(id);
       delete tabBlockMap->find(tabName)->second.first->find(id)->second;
       tabBlockMap->find(tabName)->second.first->erase(id);
   }
   else
   {
       tMap->second.second->erase(id);
       delete tabBlockMap->find(tabName)->second.second->find(id)->second;
       tabBlockMap->find(tabName)->second.second->erase(id);
   }

    writeData();
}

void DataEngine::createTabMap(const std::string &tabName)
{
    tabMap->insert(std::make_pair(tabName, std::make_pair(std::make_shared<std::map<std::string, std::array<std::string, 6>>>(), std::make_shared<std::map<std::string, std::array<std::string, 6>>>())));
    tabBlockMap->insert(std::make_pair(tabName, std::make_pair(std::make_shared<std::map<std::string, TodoBlock*>>(), std::make_shared<std::map<std::string, TodoBlock*>>())));
}

void DataEngine::removeTabMap(const std::string &tabName)
{
    this->tabMap->erase(tabName);
    tabBlockMap->find(tabName)->second.first.reset();
    tabBlockMap->find(tabName)->second.second.reset();
    this->tabBlockMap->erase(tabName);
}
