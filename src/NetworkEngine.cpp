#include "NetworkEngine.h"
//#include "TodoBlock.h"

std::shared_ptr<NetworkEngine> NetworkEngine::instance                                                                         ;

NetworkEngine::NetworkEngine(const std::string _host, const std::string _port): host(_host), port(_port), isConnected{false}
{
    std::cout<<"[!] NetworkEngine Started!"<<std::endl;
    ioc = std::make_shared<boost::asio::io_context>();
    socket = std::unique_ptr<boost::asio::ip::tcp::socket>(new boost::asio::ip::tcp::socket(*ioc));
    resolver = std::make_shared<boost::asio::ip::tcp::resolver>(*ioc);
}

void NetworkEngine::createConnection()
{
    std::cout<<"[!] Trying to Connected to server\n";
    auto resolve = resolver->resolve(host, port);
    while(true)
    {
        try
        {
            socket->connect(*resolve);
            isConnected = true;
            break;
        }
        catch (boost::system::system_error se)
        {
            std::cout<<"[e] Error in Connecting to ["<< host<<":"<<port<<"]. System_Error: "<< se.what()<<std::endl;
        }
        sleep(5);
    }
    std::cout<<"[!] Successfully Connected to ["<< host<<":"<<port<<"]. "<< socket->remote_endpoint()<<std::endl;
}

void NetworkEngine::sendBlock(TodoBlock &todoBlock)
{
    if(!isConnected)
    {
        std::cout<<"Error Not Connected To Server, Adding to queue!\n";
    }

    std::cout<<"[!] Sending Block!\n";

    Protocol::Block block(std::atol(todoBlock.id.c_str()), todoBlock.tid, todoBlock.uid, todoBlock.isToDone, todoBlock.title, todoBlock.subString);

    if(sendHeader(Protocol::TYPE::BLOCK, sizeof(block), 1))
    {
        try
        {
            auto bSize = socket->write_some(boost::asio::buffer(&block, sizeof(block)));
            std::cout<<"[!] Block Writed with Size: "<< bSize<<"\n"<< block.toJson()<< std::endl;
        }

        catch(boost::system::system_error se)
        {
            std::cout<< "[e] Error in Writing in socket, System Error: "<< se.what()<<std::endl;
        }
    }
}

bool NetworkEngine::sendHeader(Protocol::TYPE _type, uint16_t _bodySize, uint8_t _quantity)
{
    try
    {
        Protocol::Header header(Protocol::TYPE::HEADER, _type, _bodySize, _quantity);
        auto rhsize = socket->write_some(boost::asio::buffer(&header, sizeof(header)));
        std::cout<<"[>>] Header Writed with Size: "<< rhsize<<": "<< header.toJson() <<std::endl;
    }
    catch (boost::system::system_error se)
    {
        std::cout<<"[E] Exception Occured! For Header: "<< se.what()<<std::endl;
        return false;
    }
    return true;
}

void NetworkEngine::removeBlock(int64_t _id)
{
    if(sendHeader(Protocol::TYPE::DELETE_BLOCK, sizeof(int64_t), 1))
    {
            auto writeSize = socket->write_some(boost::asio::buffer(&_id, sizeof(int64_t)));
            std::cout<<"[!] Remove Block requested sent for _id: "<< _id<< " Writed Size: "<< writeSize <<std::endl;
    }
}

void NetworkEngine::getBlocks(int32_t uid)
{
    Protocol::Request::RequestBlock blockRequest(Protocol::Request::TYPE::BLOCKS_ALL_REQ, uid, "0");
    if(sendHeader(Protocol::TYPE::REQUEST, sizeof(blockRequest), 1))
    {
        auto sent_size = socket->write_some(boost::asio::buffer(&blockRequest, sizeof(blockRequest)));
        std::cout<< "[!] Request sent for Blocks with uid: "<< uid<<" size: "<< sent_size<<std::endl;
    }
    receiveData();
}

void NetworkEngine::getBlocksByTid(int32_t uid, std::string tid)
{
    Protocol::Request::RequestBlock blockRequest(Protocol::Request::TYPE::BLOCKS_ALL_REQ, uid, tid);
    if(sendHeader(Protocol::TYPE::REQUEST, sizeof(blockRequest), 1))
    {
        auto sent_size = socket->write_some(boost::asio::buffer(&blockRequest, sizeof(blockRequest)));
        std::cout<< "[!] Request sent for Blocks with uid: "<< uid<< " Size: "<< sent_size<<std::endl;
    }

    receiveData();
}

void NetworkEngine::receiveData()
{
    Protocol::Header header;
    auto hsize = socket->read_some(boost::asio::buffer(&header, sizeof(header)));
    std::cout<<"[!] Header Received size: "<<hsize<< " Type: " << (int)header.body << std::endl;

    switch(header.body)
    {
        case Protocol::TYPE::BLOCKS:
        {
                receiveBlocks(header.size, header.quantity);
                break;
        }
        default:
            std::cout<<"[E] Invalid Header Type!\n";
    }
}

void NetworkEngine::receiveBlocks(uint16_t size, uint8_t quantity)
{
    std::cout<<"[!] Receiving Blocks with size: "<< size<<" Quantity: "<< quantity<<std::endl;

    Protocol::Block blocks[quantity];
    auto bsize = socket->read_some(boost::asio::buffer(&blocks, size));
    std::cout<<"[!] Header Received with size: "<< bsize<<std::endl;

    for(auto i=0; i<quantity; i++)
        std::cout<< blocks[i].toJson()<<std::endl;
}

void NetworkEngine::removeTab(std::string tid, uint32_t _uid)
{
    if(tid.empty())
        return;

    Protocol::Request::RequestBlock request(Protocol::Request::TYPE::DELETE_TAB_REQ , _uid, tid);
    if(sendHeader(Protocol::TYPE::REQUEST, sizeof(request), 1))
    {
        auto rsize = socket->write_some(boost::asio::buffer(&request, sizeof(request)));
        std::cout<<"[>>] DeleteTab Requested for tid: "<< tid<< " Write Size: "<< rsize << std::endl;
    }
}

std::shared_ptr<NetworkEngine> NetworkEngine::getInstance(const std::string host, const std::string port)
{
    if(!instance)
    {
        std::cout<<"[!] NetworkInterface Instance Created\n";
        instance = std::make_shared<NetworkEngine>(host, port);
    }
    return instance;
}
