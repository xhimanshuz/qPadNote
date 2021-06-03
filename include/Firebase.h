#ifndef FIREBASE_H
#define FIREBASE_H

#include <stdlib.h>

#include "AbstractIO.h"
#include "firebase/variant.h"
#include "firebase/app.h"
#include "firebase/analytics.h"
#include "firebase/database.h"
#include <optional>

class DataEngine;

//TODO: Remove Raw pointers
class Firebase: public AbstractIO
{
    firebase::App *app;
    firebase::database::Database *db;
    firebase::database::DatabaseReference blockRef;

    DataEngine *dataEngine;
    std::string username;
public:
    Firebase();
    ~Firebase();
    // TODO: Cahnge the writeData name to hashSync
    void writeData();
    void writeBlock(TodoBlock &todoBlock) override;
    void writeBlocks(std::vector<TodoBlock*> &blocksVector) override;
    bool writeHeader(Protocol::TYPE _type, uint16_t _bodySize, uint8_t _quantity) override;
    void removeBlock(int64_t _id) override;

    void getBlocks(int32_t uid) override;
    void getBlocksByTid(int32_t uid, std::string tid) override;

    void receiveBlocks(uint16_t size, uint8_t quantity) override;

    void removeTab(std::string tid, uint32_t _uid = 0) override;
    void renameTab(std::string xtid, std::string tid) override;
    void addTab(const std::string& tid);

//    void removeTab(std::string tid);
    void removeBlock(int64_t id, const std::string todo, const std::string& tid);
    void fetchAll();
    void hashModified();
    void moveBlock(std::string xid, bool toogle, std::string tid, std::string id);
    inline std::optional<const firebase::database::DataSnapshot*> getSnapshot(firebase::Future<firebase::database::DataSnapshot> snapshot);

    static Firebase* getInstance();
    static Firebase *instance;

};

#endif // FIREBASE_H
