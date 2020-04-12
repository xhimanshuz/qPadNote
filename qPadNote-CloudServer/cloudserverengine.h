#ifndef CLOUDSERVERENGINE_H
#define CLOUDSERVERENGINE_H

#include <boost/asio.hpp>
#include "cloudSession.h"

class CloudServerEngine
{
    boost::asio::io_context *ioc;
    boost::asio::ip::tcp::acceptor *acceptor;
    std::vector<CloudSession*> *sessionVector;

public:
    CloudServerEngine(boost::asio::io_service *_ioc);
    void acceptConnection();

};

#endif // CLOUDSERVERENGINE_H
