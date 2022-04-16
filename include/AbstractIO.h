#ifndef ABSTRACTIO_H
#define ABSTRACTIO_H

#include "TodoBlock.h"

class AbstractIO {
public:
  virtual void addTab(const std::string &tid) = 0;
  virtual void fetchAll() = 0;
  virtual void getBlocks(int32_t uid) = 0;
  virtual void getBlocksByTid(int32_t uid, std::string tid) = 0;
  virtual void hashModified() = 0;
  virtual void moveBlock(std::string xid, bool toogle, std::string tid,
                         std::string id) = 0;

  virtual void receiveBlocks(uint16_t size, uint8_t quantity) = 0;
  // TODO: Have to remove uid
  virtual void removeBlock(int64_t _id) = 0;
  virtual void removeBlock(int64_t id, const std::string todo,
                           const std::string &tid) = 0;
  virtual void removeTab(std::string tid, uint32_t _uid = 0) = 0;

  virtual void renameTab(std::string xtid, std::string tid) = 0;

  virtual void writeData(QByteArray const& data) = 0;
  virtual void writeBlock(TodoBlock &todoBlock) = 0;
  virtual void writeBlocks(std::vector<TodoBlock *> &blocksVector) = 0;

  virtual void readData(QByteArray& data) = 0;
};

#endif // ABSTRACTIO_H
