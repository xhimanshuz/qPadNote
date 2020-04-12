#ifndef DATAENGINE_H
#define DATAENGINE_H

#include "TodoBlock.h"
#include "NetworkEngine.h"

#include <QMap>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDir>

class DataEngine
{
    std::string fileName;
public:
    DataEngine();
    ~DataEngine();
    static DataEngine *instance;
    static DataEngine *getInstance();
    QJsonDocument mapToJson();
    void jsonToMap(QJsonObject jObj);
    void readData();
    void writeData();
    void deleteBlock(std::string id);

    //<id, [title, subString, isTodo, id]>
    std::map<std::string, std::array<std::string, 4>> *todoMap;
    std::map<std::string, std::array<std::string, 4>> *toDoneMap;

    std::map<std::string, TodoBlock*> *todoBlockMap;
    std::map<std::string, TodoBlock*> *toDoneBlockMap;

    NetworkEngine *networkEngine;
};

#endif // DATAENGINE_H
