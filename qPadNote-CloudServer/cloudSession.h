#ifndef CLOUDSESSION_H
#define CLOUDSESSION_H

#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "data.h"

class CloudSession
{
    boost::asio::ip::tcp::socket socket;

public:
    CloudSession(boost::asio::ip::tcp::socket _socket);
    std::string ptreeToStr(boost::property_tree::ptree &ptree);

    void readRequestType();
    std::string writeJsonRequest();
    std::string readJsonRequest();
    std::string readJsonFile();
    void writeJson(boost::property_tree::ptree json);
    int readId();
    std::string responseJson(std::string errorStr, error_t ec,bool response = false, boost::property_tree::ptree *json = nullptr);

    boost::asio::ip::tcp::socket *getSocket();
};

#endif // CLOUDSESSION_H
