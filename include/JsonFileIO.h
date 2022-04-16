#ifndef JSONFILEIO_H
#define JSONFILEIO_H

#include "Log.h"

#include <AbstractIO.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

class JsonFileIO : public AbstractIO
{
  JsonFileIO(): m_filename{"qPadNote.json"} {
    log = spdlog::get("qlog");
    log->info("[!] JsonFileIO Initialized");
  }

  QString m_filename;
  std::shared_ptr<spdlog::logger> log;
 public:
  static JsonFileIO* instance;
  static JsonFileIO* getInstance();

  void addTab(const std::string &tid) override;
  void fetchAll() override;
  void getBlocks(int32_t uid) override;
  void getBlocksByTid(int32_t uid, std::string tid) override;
  void hashModified() override;
  void moveBlock(std::string xid, bool toogle, std::string tid,
                 std::string id) override;

  void receiveBlocks(uint16_t size, uint8_t quantity) override;
  void removeBlock(int64_t _id) override;
  void removeBlock(int64_t id, const std::string todo,
                   const std::string &tid) override;
  void removeTab(std::string tid, uint32_t _uid = 0) override;

  void renameTab(std::string xtid, std::string tid) override;

  void readData(QByteArray& data) override;

  void writeData(QByteArray const& data) override;
  void writeBlock(TodoBlock &todoBlock) override;
  void writeBlocks(std::vector<TodoBlock *> &blocksVector) override;

  QString readFileString(QString const& fileName);
  QJsonObject readFileJson(QString const& fileName);
  QJsonObject stringToJson(QString const& str);
  QJsonObject stringToJson(QByteArray const& str);
  QByteArray readFile(QString const& filename);

};

#endif // JSONFILEIO_H
