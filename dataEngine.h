#ifndef DATAENGINE_H
#define DATAENGINE_H

#include "TodoBlock.h"

#include <QMap>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDir>

class DataEngine
{
    QString fileName;
public:
    DataEngine();

    static DataEngine *instance;
    static DataEngine *getInstance();
    QJsonDocument mapToJson();
    void jsonToMap(QJsonObject jObj);
    void readData();
    void writeData();

    //<id, [title, subString, isTodo, id]>
    QMap<QString, QStringList> *todoMap;
    QMap<QString, QStringList> *toDoneMap;

    QMap<QString, TodoBlock*> *todoBlockMap;
    QMap<QString, TodoBlock*> *toDoneBlockMap;
};

#endif // DATAENGINE_H
