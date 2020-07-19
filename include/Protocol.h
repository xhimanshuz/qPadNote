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
    RESPONSE = 'R',
    REQUEST = 'r'
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
};

namespace Request
{
enum TYPE
{
    BLOCKS_TAB_REQ = 100,
    BLOCKS_ALL_REQ
};

struct BlockRequest
{
    Protocol::Request::TYPE type;
    int16_t uid;
    char tid[10];
    BlockRequest(Protocol::Request::TYPE _type, int16_t _uid, std::string _tid): type(_type), uid(_uid)
    {
        std::strcpy(tid, _tid.c_str());
    }
};

}

}

#endif // PROTOCOL_H
