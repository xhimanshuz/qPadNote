#include "JsonFileIO.h"

JsonFileIO* JsonFileIO::instance = nullptr;

JsonFileIO *JsonFileIO::getInstance()
{
  if(!instance)
    instance = new JsonFileIO();
  return instance;
}

void JsonFileIO::addTab(const std::string &tid)
{

}

void JsonFileIO::fetchAll()
{

}

void JsonFileIO::getBlocks(int32_t uid)
{

}

void JsonFileIO::getBlocksByTid(int32_t uid, std::string tid)
{

}

void JsonFileIO::hashModified()
{

}

void JsonFileIO::moveBlock(std::string xid, bool toogle, std::string tid, std::string id)
{

}

void JsonFileIO::receiveBlocks(uint16_t size, uint8_t quantity)
{

}

void JsonFileIO::removeBlock(int64_t _id)
{

}

void JsonFileIO::removeBlock(int64_t id, const std::string todo, const std::string &tid)
{

}

void JsonFileIO::removeTab(std::string tid, uint32_t _uid)
{

}

void JsonFileIO::renameTab(std::string xtid, std::string tid)
{

}

void JsonFileIO::readData(QByteArray& data)
{
  QFile file(m_filename);
  if(!file.open(QFile::ReadOnly))
  {
    qDebug()<<" Error I/O Read File "<< m_filename;
    data = {};
    // Writing dummy data.
    QJsonObject json =
        {
            {"appData",
             QJsonObject{
                 {"fontFamily","Roboto"},
                 {"fontSize", 13}
             }},
            {"userData",
             QJsonObject{}}
        };

    writeData(QJsonDocument(json).toJson());
    return;
  }

  data = file.readAll();
}

void JsonFileIO::writeData(QByteArray const& data)
{
  QFile file(m_filename);
  if(!file.open(QFile::WriteOnly))
  {
    qDebug()<<" Error I/O Writing File "<< m_filename;
    return;
  }

  file.write(data);
  file.close();
}

void JsonFileIO::writeBlock(TodoBlock &todoBlock)
{

}

void JsonFileIO::writeBlocks(std::vector<TodoBlock *> &blocksVector)
{

}

QString JsonFileIO::readFileString(const QString &fileName)
{
  return "";
}

QJsonObject JsonFileIO::readFileJson(const QString &fileName) {
  auto byteFile = readFile(fileName);
  return stringToJson(byteFile);
}

QJsonObject JsonFileIO::stringToJson(const QString &str) {
  return stringToJson(str.toLocal8Bit());
}

QJsonObject JsonFileIO::stringToJson(const QByteArray &str) {
  QJsonDocument doc = QJsonDocument::fromJson(str);
  if(doc.isNull())
    return {};
  return doc.object();
}

QByteArray JsonFileIO::readFile(const QString &filename) {
  QFile file(filename);
  if(!file.open(QFile::ReadOnly))
    return {};

  return file.readAll();
}
