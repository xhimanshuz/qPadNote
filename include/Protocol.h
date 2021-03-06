#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <inttypes.h>
#include <limits>

#include "Block.h"

namespace Protocol
{

enum class TYPE
{
    HEADER = 'H',
    BLOCK = 'B',
    BLOCKS = 'b',
    DELETE_BLOCK = 'd',
    DELETE_TAB = 't',
    RESPONSE = 'R',
    REQUEST = 'r',
    HASH_EXCHANGE = 'x'
};

struct Header
{
    TYPE type;
    bool isRequest;
    TYPE body;
    uint16_t size;
    uint8_t quantity;
    Header(TYPE _type, TYPE _body, uint16_t _size, uint8_t _quantity): type(_type), body(_body), size(_size), quantity(_quantity) { }
    Header() {}
    std::string toJson()
    {
        boost::property_tree::ptree json;
        json.put("type", char(type));
        json.put("body", char(body));
        json.put("size", size);
        json.put("quantity", quantity);
        json.put("isRequest", isRequest);

        std::stringstream ss;
        boost::property_tree::write_json(ss, json);

        return ss.str();
    }
};

namespace Request
{
enum TYPE
{
    BLOCKS_TAB_REQ = 100,
    BLOCKS_ALL_REQ,
    DELETE_TAB_REQ,
    RENAME_TAB_REQ
};

struct RequestBlock
{
    Protocol::Request::TYPE type;
    int16_t uid;
    char tid[10];
    RequestBlock(Protocol::Request::TYPE _type, int16_t _uid, std::string _tid): type(_type), uid(_uid)
    {
        std::memset(tid, 0, sizeof(tid));
        std::strcpy(tid, _tid.c_str());
    }
    RequestBlock(){};
};

struct DeleteTabRequest
{
    Protocol::Request::TYPE type = Request::TYPE::DELETE_TAB_REQ;
    char tid[10];
    DeleteTabRequest(std::string _tid)
    {
        std::memset(tid, 0, sizeof(tid));
        std::strcpy(tid, _tid.c_str());
    }
    DeleteTabRequest()
    {
        std::memset(tid, 0, sizeof(tid));
    }
};

struct RenameTabRequest
{
    Protocol::Request::TYPE type = Request::TYPE::RENAME_TAB_REQ;
    char xtid[10];
    char tid[10];

    RenameTabRequest(std::string _xtid, std::string _tid)
    {
        std::memset(xtid, 0, sizeof(xtid));
        std::memset(tid, 0, sizeof(tid));
        std::strcpy(xtid, _xtid.c_str());
        std::strcpy(tid, _tid.c_str());
    }

    RenameTabRequest()
    {
        std::memset(xtid, 0, sizeof(xtid));
        std::memset(tid, 0, sizeof(tid));
    }
};

struct HashExchange
{
    Protocol::TYPE type = Protocol::TYPE::HASH_EXCHANGE;
    int16_t uid;
    uint16_t hashQty;
    HashExchange(int16_t _uid, uint16_t _hashQty): uid{_uid}, hashQty{_hashQty} {}
    HashExchange(){}
};

}

}

#endif // PROTOCOL_H
