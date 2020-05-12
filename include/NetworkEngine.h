#ifndef NETWORKENGINE_H
#define NETWORKENGINE_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpSocket>
#include <QNetworkReply>

#include "data.h"

class NetworkEngine : public QObject
{
    Q_OBJECT
public:
    explicit NetworkEngine(QObject *parent = nullptr);
    ~NetworkEngine();

    QJsonObject requestJson(QString id);
    void writeJson(QJsonObject obj);
    QTcpSocket socket;
signals:
    void jsonReceived(QJsonObject jobj);

};

#endif // NETWORKENGINE_H
