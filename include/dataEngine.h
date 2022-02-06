#ifndef DATAENGINE_H
#define DATAENGINE_H

#include <array>
#include <memory>

#include "TodoBlock.h"

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
    std::string username;

    Config(): fontSize(13), fontFamily("Roboto")
    {

    }
};

class DataEngine
{
    QJsonObject readData();
    QJsonDocument mapToJson();

    std::string fileName;
public:
 void jsonToMap(QJsonObject jObj);
    DataEngine();
    ~DataEngine();
    void writeData();
    void deleteBlock(int64_t id, const std::string tabName);
    void createTabMap(const std::string& tabName);
    void removeTabMap(const std::string& tabName);
    void renameTabMap(const std::string& oldName, const std::string& newName);
    void hashModified();
    void syncWithNetwork();
    const std::string& readUsername();
    std::shared_ptr<std::map<std::string, std::pair< std::shared_ptr<std::map<int64_t, TodoBlock*>>, std::shared_ptr<std::map<int64_t, TodoBlock*>>> >> tabBlockMap;


    static DataEngine* instance;
    static DataEngine* getInstance();

    Config config;
    void writeData(QJsonDocument json);

};

#endif // DATAENGINE_H
