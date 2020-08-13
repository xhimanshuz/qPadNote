#ifndef BLOCK_H
#define BLOCK_H

#ifdef SERVER

#include <bsoncxx/document/value.hpp>
#include <bsoncxx/document/view.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>

#endif

#ifndef SERVER

#include "TodoBlock.h"

#endif

#include <iostream>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace Protocol
{

class Block
{
public:
    int64_t _id;
    bool isDone;
    char tid[10];
    char title[12];
    char substring[64];
    int16_t uid;
    uint32_t hash;

    Block(int64_t _id, const std::string tid, int32_t uid, bool isDone, const std::string title, const std::string substring, uint32_t _hash);
    Block();
    ~Block();

#ifndef SERVER
    Block(TodoBlock &todoBlock);
    Block& operator= (TodoBlock *todoBlock);
    Block& operator= (TodoBlock &todoBlock);
#endif
#ifdef SERVER

    Block(bsoncxx::document::view block);
    bsoncxx::document::value toDocumentValue();
    bool fillData(bsoncxx::document::view block);

#endif
    const std::string toString();
    const std::string toJson();

};
}

#endif // BLOCK_H
