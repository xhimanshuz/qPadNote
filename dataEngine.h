#ifndef DATAENGINE_H
#define DATAENGINE_H

#include "TodoBlock.h"

#include <QMap>

class DataEngine
{
public:
    DataEngine();

    static DataEngine *instance;
    static DataEngine *getInstance();

    //<id, (title, subString, isTodo)>
    QMap<QString, QStringList> *todoMap;
    QMap<QString, QStringList> *toDoneMap;

    QMap<QString, TodoBlock*> *todoBlockMap;
    QMap<QString, TodoBlock*> *toDoneBlockMap;
};

#endif // DATAENGINE_H
