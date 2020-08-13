#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H

#include <boost/asio.hpp>
#include <iostream>

#include "AbstractIO.h"

class NetworkEngine: public AbstractIO
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
    void writeBlock(TodoBlock &todoBlock) override;
    void writeBlocks(std::vector<TodoBlock*> &blocksVector) override;
    bool writeHeader(Protocol::TYPE _type, uint16_t _bodySize, uint8_t _quantity) override;
    void removeBlock(int64_t _id) override;

    void getBlocks(int32_t uid) override;
    void getBlocksByTid(int32_t uid, std::string tid) override;

    void receiveData();
    void receiveBlocks(uint16_t size, uint8_t quantity) override;
    Protocol::Header readHeader();

    void removeTab(std::string tid, uint32_t _uid = 0) override;
    void renameTab(std::string xtid, std::string tid) override;

    static std::shared_ptr<NetworkEngine> instance;
    static std::shared_ptr<NetworkEngine> getInstance(const std::string host="", const std::string port="");

    void hashSync(int16_t uid, std::vector<uint32_t> &hash);
    void writeHashVector(int16_t uid, std::vector<uint32_t> &hashVector);
    std::vector<uint32_t> readHashVector();
};

#endif // NETWORKINTERFACE_H
