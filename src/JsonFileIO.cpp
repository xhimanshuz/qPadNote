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
  _FUNC_LOG_
}

void JsonFileIO::fetchAll()
{
  _FUNC_LOG_
}

void JsonFileIO::getBlocks(int32_t uid)
{
  _FUNC_LOG_
}

void JsonFileIO::getBlocksByTid(int32_t uid, std::string tid)
{
  _FUNC_LOG_
}

void JsonFileIO::hashModified()
{
  _FUNC_LOG_
}

void JsonFileIO::moveBlock(std::string xid, bool toogle, std::string tid, std::string id)
{
  _FUNC_LOG_
}

void JsonFileIO::receiveBlocks(uint16_t size, uint8_t quantity)
{
  _FUNC_LOG_
}

void JsonFileIO::removeBlock(int64_t _id)
{
  _FUNC_LOG_
}

void JsonFileIO::removeBlock(int64_t id, const std::string todo, const std::string &tid)
{
  _FUNC_LOG_
}

void JsonFileIO::removeTab(std::string tid, uint32_t _uid)
{
  _FUNC_LOG_
}

void JsonFileIO::renameTab(std::string xtid, std::string tid)
{
  _FUNC_LOG_
}

void JsonFileIO::readData(QByteArray& data)
{
  _FUNC_LOG_
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
  _FUNC_LOG_
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
  _FUNC_LOG_
}

void JsonFileIO::writeBlocks(std::vector<TodoBlock *> &blocksVector)
{
  _FUNC_LOG_
}

QString JsonFileIO::readFileString(const QString &fileName)
{
  _FUNC_LOG_
  return "";
}

QJsonObject JsonFileIO::readFileJson(const QString &fileName) {
  _FUNC_LOG_
  auto byteFile = readFile(fileName);
  return stringToJson(byteFile);
}

QJsonObject JsonFileIO::stringToJson(const QString &str) {
  _FUNC_LOG_
  return stringToJson(str.toLocal8Bit());
}

QJsonObject JsonFileIO::stringToJson(const QByteArray &str) {
  _FUNC_LOG_
  QJsonDocument doc = QJsonDocument::fromJson(str);
  if(doc.isNull())
    return {};
  return doc.object();
}

QByteArray JsonFileIO::readFile(const QString &filename) {
  _FUNC_LOG_
  QFile file(filename);
  if(!file.open(QFile::ReadOnly))
    return {};

  return file.readAll();
}
