#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H

#include <boost/asio.hpp>
//#include <boost/asio/io_context.hpp>
#include <iostream>
#include "TodoBlock.h"


#include "Protocol.h"

class NetworkEngine
{
    std::unique_ptr<boost::asio::ip::tcp::socket> socket;
    std::shared_ptr<boost::asio::ip::tcp::resolver> resolver;
    std::string host;
    std::string port;

//    std::deque<std::string> dataqueu; // IF SOCKET IS NOT CONNECTED OR SOCKET IS BUSSY
public:
    bool isConnected;
    std::shared_ptr<boost::asio::io_context> ioc;
    NetworkEngine(const std::string host, const std::string port);
    void createConnection();
    void sendBlock(TodoBlock &todoBlock);
    bool sendHeader(Protocol::TYPE _type, uint16_t _bodySize, uint8_t _quantity);
    void removeBlock(int64_t _id);

    void getBlocks(int32_t uid);
    void getBlocksByTid(int32_t uid, std::string tid);

    void receiveData();
    void receiveBlocks(uint16_t size, uint8_t quantity);

    void removeTab(std::string tid, uint32_t _uid = 0);
    void renameTab(std::string xtid, std::string tid);

    static std::shared_ptr<NetworkEngine> instance;
    static std::shared_ptr<NetworkEngine> getInstance(const std::string host="", const std::string port="");

};

#endif // NETWORKINTERFACE_H
