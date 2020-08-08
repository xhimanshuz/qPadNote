#ifndef DATAENGINE_H
#define DATAENGINE_H

#include <array>
#include <memory>

#include "TodoBlock.h"
//#include "NetworkEngine.h"

#include <QMap>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDir>

class TodoWindow;

struct Config
{
    int fontSize = 13;
    std::string fontFamily = "Roboto";

    Config(): fontSize(13), fontFamily("Roboto")
    {

    }
};

class DataEngine
{
    void jsonToMap(QJsonObject jObj);
    QJsonObject readData();
    QJsonDocument mapToJson();

    std::string fileName;
//    NetworkEngine *networkEngine;
public:
    DataEngine();
    ~DataEngine();
    void writeData();
    void deleteBlock(std::string id, const std::string tabName);
    void createTabMap(const std::string& tabName);
    void removeTabMap(const std::string& tabName);
    void renameTabMap(const std::string& oldName, const std::string& newName);

    std::shared_ptr<std::map<std::string, std::pair< std::shared_ptr<std::map<std::string, TodoBlock*>>, std::shared_ptr<std::map<std::string, TodoBlock*>>> >> tabBlockMap;

    static DataEngine* instance;
    static DataEngine* getInstance();

    Config config;

};

#endif // DATAENGINE_H
