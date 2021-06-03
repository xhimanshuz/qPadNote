#ifndef ABSTRACTIO_H
#define ABSTRACTIO_H

#include "TodoBlock.h"
#include "Protocol.h"
//class TodoBlock;
//class Protocol;

class AbstractIO
{
public:

    virtual void writeBlock(TodoBlock &todoBlock) = 0;
    virtual void writeBlocks(std::vector<TodoBlock*> &blocksVector) = 0;
    virtual bool writeHeader(Protocol::TYPE _type, uint16_t _bodySize, uint8_t _quantity) = 0;
    virtual void removeBlock(int64_t _id) = 0;

    virtual void getBlocks(int32_t uid) = 0;
    virtual void getBlocksByTid(int32_t uid, std::string tid) = 0;

//    virtual void receiveData() {};
    virtual void receiveBlocks(uint16_t size, uint8_t quantity) = 0;
    // TODO: Have to remove uid
    virtual void removeTab(std::string tid, uint32_t _uid = 0) = 0;
    virtual void renameTab(std::string xtid, std::string tid) = 0;
};

#endif // ABSTRACTIO_H
