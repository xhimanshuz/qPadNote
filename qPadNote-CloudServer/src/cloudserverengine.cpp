#include "cloudserverengine.h"

CloudServerEngine::CloudServerEngine(boost::asio::io_service *_ioc): ioc(_ioc)
{
    acceptor = new boost::asio::ip::tcp::acceptor(*ioc, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8001));
    sessionVector = new std::vector<CloudSession*>;
    std::cout<<"SERVER STARTED!"<< std::endl;
    acceptConnection();
}

void CloudServerEngine::acceptConnection()
{
    for(auto v = 0; v < sessionVector->size(); v++)
    {
        auto socket = sessionVector->at(v)->getSocket();
        if(socket->is_open())
            std::cout<<"Opened "<<socket->remote_endpoint().address().to_string() <<socket->remote_endpoint().port()<<std::endl;
        else
        {
            std::cout<<"Closed! "<<socket->remote_endpoint().address().to_string() <<socket->remote_endpoint().port()<<std::endl;
            auto it = std::find(sessionVector->begin(), sessionVector->end(), sessionVector->at(v));
            delete *it;
            sessionVector->erase(it);
        }
    }
    std::cout << "WAITING TO ACCEPT CONNECTION"<<std::endl;

    acceptor->async_accept([this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket){
        std::cout<<"Accepted! at "<< socket.remote_endpoint().address()<<":"<<socket.remote_endpoint().port()<<"-"<<socket.remote_endpoint().protocol().protocol()<<std::endl;
        if(!ec)
        {
            auto s = new CloudSession(std::move(socket));
            s->getSocket();
            sessionVector->push_back(s);
        }
        else
        {
            std::cerr<<"Error Occoured"<< ec.message()<<std::endl;
        }
        acceptConnection();
    });
}
