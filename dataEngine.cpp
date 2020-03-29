#include "dataEngine.h"
#include <QDebug>

DataEngine* DataEngine::instance = nullptr;

DataEngine::DataEngine(): fileName{"setting.json"}
{
    todoMap = new QMap<QString, QStringList>;
    toDoneMap = new QMap<QString, QStringList>;
    todoBlockMap = new QMap<QString, TodoBlock*>;
    toDoneBlockMap = new QMap<QString, TodoBlock*>;

    readData();
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
        block.insert("title", todo.at(0));
        block.insert("subString", todo.at(1));
        block.insert("isTodo", false);
        block.insert("id", todo.at(3));
        todoJ.insert(todo.at(3), block);

    }
    mainJ.insert("todo", todoJ);

    QJsonObject toDoneJ;
    for(auto toDone: *toDoneMap)
    {
        QJsonObject block;
        block.insert("title", toDone.at(0));
        block.insert("subString", toDone.at(1));
        block.insert("isTodo", true);
        block.insert("id", toDone.at(3));
        toDoneJ.insert(toDone.at(3), block);
    }
    mainJ.insert("toDone", toDoneJ);

    return QJsonDocument(mainJ);
}

void DataEngine::jsonToMap(QJsonObject jObj)
{
    QJsonObject todoJ = jObj.value("todo").toObject();
    for(auto todo: todoJ)
    {
        QString id = todo.toObject().value("id").toString();
        QString title = todo.toObject().value("title").toString();
        QString subString = todo.toObject().value("subString").toString();
        bool isToDone = false;
        todoMap->insert(id, QStringList()<< title<< subString<< ((isToDone)?"1":"0")<< id);
    }

    QJsonObject toDoneJ = jObj.value("toDone").toObject();
    for(auto toDone: toDoneJ)
    {
        QString id = toDone.toObject().value("id").toString();
        QString title = toDone.toObject().value("title").toString();
        QString subString = toDone.toObject().value("subString").toString();
        bool isToDone = true;
        toDoneMap->insert(id, QStringList()<< title<< subString<< ((isToDone)?"1":"0")<< id);
    }

}

void DataEngine::readData()
{
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly))
    {
        qDebug()<<" Error I/O Read File "<< fileName;
        return;
    }

    QJsonDocument jDoc = QJsonDocument::fromJson(file.readAll());
    file.close();

    jsonToMap(jDoc.object());
}

void DataEngine::writeData()
{
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly))
    {
        qDebug()<<" Error I/O Writing File "<< fileName;
        return;
    }

    file.write(mapToJson().toJson());
    file.close();
}
