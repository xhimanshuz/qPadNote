#include "dataEngine.h"

DataEngine* DataEngine::instance = nullptr;

DataEngine::DataEngine()
{
    todoMap = new QMap<QString, QStringList>;
    toDoneMap = new QMap<QString, QStringList>;
    todoBlockMap = new QMap<QString, TodoBlock*>;
    toDoneBlockMap = new QMap<QString, TodoBlock*>;
}

DataEngine* DataEngine::getInstance()
{
    if(!instance)
        instance = new DataEngine;
    return instance;
}
