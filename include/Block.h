#ifndef BLOCK_H
#define BLOCK_H

#ifdef SERVR

#include <bsoncxx/document/value.hpp>
#include <bsoncxx/document/view.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>

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

    Block(int64_t _id, const std::string tid, int32_t uid, bool isDone, const std::string title, const std::string substring);
    Block();
    ~Block();
#ifdef SERVR

    Block(bsoncxx::document::view block);
    bsoncxx::document::value toDocumentValue();
    bool fillData(bsoncxx::document::view block);

#endif
    const std::string toString();
    const std::string toJson();

};
}

#endif // BLOCK_H
