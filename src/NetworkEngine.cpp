#include "NetworkEngine.h"
#include <QDebug>
#include <iostream>

NetworkEngine::NetworkEngine(QObject *parent) : QObject(parent)
{
    socket.connectToHost("localhost", 8001);
}

NetworkEngine::~NetworkEngine()
{
    socket.close();
}

QJsonObject NetworkEngine::requestJson(QString id)
{
    socket.write("R");
    socket.write(id.toUtf8());
    socket.waitForBytesWritten();
    QString s;
    connect(&socket, &QTcpSocket::readyRead, [&]{
        qDebug()<<"Waiting to read";
//        socket.waitForReadyRead();
        auto read = socket.readAll();
        QJsonDocument jdoc = QJsonDocument::fromJson(read);
        auto responseJson = jdoc.object();
        if(responseJson.value("response").toVariant().toBool())
            emit jsonReceived(responseJson.value("json").toObject());
    });
    return QJsonObject();
}

void NetworkEngine::writeJson(QJsonObject obj)
{
    QJsonDocument jDoc;
    QJsonObject idJ;
    idJ.insert("12345", obj);
    jDoc.setObject(idJ);
//    socket.connectToHost("127.0.0.1",8001);
    socket.write("W");
    size_t size = jDoc.toJson().size();
    QByteArray ba = QString::number(size).toUtf8();
    socket.write(ba);
    socket.write(jDoc.toJson());
    socket.waitForReadyRead();
    auto response = socket.readAll();
    QJsonDocument jDocResponse = QJsonDocument::fromJson(response);
    auto jObjR = jDocResponse.object();
}
