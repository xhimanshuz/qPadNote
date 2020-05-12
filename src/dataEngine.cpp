#include "dataEngine.h"
#include <QDebug>

DataEngine* DataEngine::instance = nullptr;

DataEngine::DataEngine(): fileName{"setting.json"}, received(false)
{
    todoMap = new std::map<std::string, std::array<std::string, 4>>;
    toDoneMap = new std::map<std::string, std::array<std::string, 4>>;
    todoBlockMap = new std::map<std::string, TodoBlock*>;
    toDoneBlockMap = new std::map<std::string, TodoBlock*>;
    networkEngine = new NetworkEngine;

    readData();
}

DataEngine::~DataEngine()
{
//    delete networkEngine;
}



DataEngine* DataEngine::getInstance()
{
    if(!instance)
        instance = new DataEngine;
    return instance;
}

QJsonDocument DataEngine::mapToJson()
{
    QJsonObject mainJ;

    QJsonObject todoJ;
    for(auto todo: *todoMap)
    {
        QJsonObject block;
        block.insert("title", todo.second.at(0).c_str());
        block.insert("subString", todo.second.at(1).c_str());
        block.insert("isTodo", false);
        block.insert("id", todo.second.at(3).c_str());
        todoJ.insert(todo.second.at(3).c_str(), block);
    }
    mainJ.insert("todo", todoJ);

    QJsonObject toDoneJ;
    for(auto toDone: *toDoneMap)
    {
        QJsonObject block;
        block.insert("title", toDone.second.at(0).c_str());
        block.insert("subString", toDone.second.at(1).c_str());
        block.insert("isTodo", true);
        block.insert("id", toDone.second.at(3).c_str());
        toDoneJ.insert(toDone.second.at(3).c_str(), block);
    }
    mainJ.insert("toDone", toDoneJ);

//    networkEngine->writeJson(mainJ);
    return QJsonDocument(mainJ);
}

void DataEngine::jsonToMap(QJsonObject jObj)
{
    QJsonObject todoJ = jObj.value("todo").toObject();
    for(auto todo: todoJ)
    {
        std::string id = todo.toObject().value("id").toString().toStdString();
        std::string title = todo.toObject().value("title").toString().toStdString();
        std::string subString = todo.toObject().value("subString").toString().toStdString();
        bool isToDone = false;
        todoMap->insert(std::pair<std::string, std::array<std::string, 4>>(id, {title, subString, ((isToDone)?"1":"0"), id}));
    }

    QJsonObject toDoneJ = jObj.value("toDone").toObject();
    for(auto toDone: toDoneJ)
    {
        std::string id = toDone.toObject().value("id").toString().toStdString();
        std::string title = toDone.toObject().value("title").toString().toStdString();
        std::string subString = toDone.toObject().value("subString").toString().toStdString();
        bool isToDone = true;
        toDoneMap->insert(std::pair<std::string, std::array<std::string, 4>>(id, {title, subString, ((isToDone)?"1":"0"), id}));
    }

}

void DataEngine::readData()
{
//    auto json = networkEngine->requestJson("12345");
//    QObject::connect(networkEngine, &NetworkEngine::jsonReceived, [this](QJsonObject jObj){
//            jsonToMap(jObj);
//            received = true;
//    });


//    if(json.size() > 0)
//    {
//        jsonToMap(json);
//        return;
//    }

    QFile file(fileName.c_str());
    if(!file.open(QFile::ReadOnly))
    {
        qDebug()<<" Error I/O Read File "<< fileName.c_str();
        return;
    }

    QJsonDocument jDoc = QJsonDocument::fromJson(file.readAll());
    file.close();
    jsonToMap(jDoc.object());
}

void DataEngine::writeData()
{
    QFile file(fileName.c_str());
    if(!file.open(QFile::WriteOnly))
    {
        qDebug()<<" Error I/O Writing File "<< fileName.c_str();
        return;
    }

    updateMap();
    file.write(mapToJson().toJson());
    file.close();
}

void DataEngine::deleteBlock(std::string id)
{
    auto it = todoMap->find(id);
    if(it != todoMap->end())
    {
        todoMap->erase(id);
        delete todoBlockMap->find(id)->second;
        todoBlockMap->erase(id);
    }
    it = toDoneMap->find(id);
    if(it != toDoneMap->end())
    {
        toDoneMap->erase(id);
        delete toDoneBlockMap->find(id)->second;
        toDoneBlockMap->erase(id);
    }
    writeData();
}

void DataEngine::updateMap()
{
    for(auto &todoBlock: *todoBlockMap)
    {
        todoMap->at(todoBlock.first).at(0) = todoBlock.second->title;
        todoMap->at(todoBlock.first).at(1) = todoBlock.second->getSubString();
        todoMap->at(todoBlock.first).at(2) = "0";
        todoMap->at(todoBlock.first).at(3) = todoBlock.second->id;
    }
    for(auto &toDoneBlock: *toDoneBlockMap)
    {
        toDoneMap->at(toDoneBlock.first).at(0) = toDoneBlock.second->title;
        toDoneMap->at(toDoneBlock.first).at(1) = toDoneBlock.second->getSubString();
        toDoneMap->at(toDoneBlock.first).at(2) = "1";
        toDoneMap->at(toDoneBlock.first).at(3) = toDoneBlock.second->id;
    }

}
