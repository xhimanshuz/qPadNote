#ifndef DATAENGINE_H
#define DATAENGINE_H

#include "TodoBlock.h"
#include "NetworkEngine.h"

#include <QMap>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDir>

class TodoWindow;

class DataEngine
{
    void jsonToMap(QJsonObject jObj);
    QJsonObject readData();
    QJsonDocument mapToJson();


    std::string fileName;
    NetworkEngine *networkEngine;
public:
    DataEngine();
    ~DataEngine();
    void writeData();
    void deleteBlock(std::string id, const std::string tabName);
    void createTabMap(const std::string& tabName);
    void removeTabMap(const std::string& tabName);

    std::shared_ptr<std::map<std::string, std::pair<std::shared_ptr<std::map<std::string, std::array<std::string, 6> >>, std::shared_ptr<std::map<std::string, std::array<std::string, 6> >>>>> tabMap;
    std::shared_ptr<std::map<std::string, std::pair< std::shared_ptr<std::map<std::string, TodoBlock*>>, std::shared_ptr<std::map<std::string, TodoBlock*>> >>> tabBlockMap;

    static std::shared_ptr<DataEngine> instance;
    static std::shared_ptr<DataEngine> getInstance();

};

#endif // DATAENGINE_H
