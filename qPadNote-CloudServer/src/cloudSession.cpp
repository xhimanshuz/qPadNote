#include "cloudSession.h"
using namespace std::literals::chrono_literals;

CloudSession::CloudSession(boost::asio::ip::tcp::socket _socket): socket(std::move(_socket))
{
    readRequestType();
}

std::string CloudSession::ptreeToStr(boost::property_tree::ptree &ptree)
{
    std::stringstream ss;
    boost::property_tree::write_json(ss, ptree);
    return ss.str();
}

void CloudSession::readRequestType()
{
    std::cout<<"Reading Request Type"<<std::endl;
    char rType[1];
    socket.read_some(boost::asio::buffer(&rType, 1));
    std::cout<<"Request Read Type: "<< rType[0]<<std::endl;
    std::string json;
    std::this_thread::sleep_for(10s);
    switch (rType[0])
    {
    case REQUEST_TYPE::Read:
    {
        json = readJsonRequest();
        break;
    }
    case REQUEST_TYPE::Write:
    {
        json = writeJsonRequest();
    }
    default:
    {

        json = responseJson("Invalid Request Type", -1);
    }
    }

//    std::cout<<"SENDING\n"<<json<<std::endl;
    socket.write_some(boost::asio::buffer(json, json.size()));
//    socket.close();
    return;
}

std::string CloudSession::writeJsonRequest()
{
    std::cout<<"Writing Json Request"<<std::endl;
    char size[5];
    auto readSize = socket.read_some(boost::asio::buffer(&size, sizeof(size)));
    char json[atoi(size)];
    boost::asio::streambuf buf;
    readSize = socket.read_some(boost::asio::buffer(&json, atoi(size)));
    json[0] = '{';
    json[atoi(size)-1] = '\0';
    std::stringstream ss(json);
    boost::property_tree::ptree ptree;
    try
    {
        boost::property_tree::read_json(ss, ptree);
    }
    catch (boost::system::system_error ec)
    {
        return responseJson(ec.what(), ec.code().value());
    }

//    std::cout<<json<<" " <<std::endl;
    writeJson(ptree);

    return responseJson("NULL", 0, true);
}

std::string CloudSession::readJsonRequest()
{
    std::cout<<"Reading Json Request"<<std::endl;
    auto id = readId();
    if(!id)
        return "Invalid";

    boost::property_tree::ptree ptree;
    std::stringstream ss;
    boost::property_tree::json_parser::read_json("setting.json", ptree);
    boost::property_tree::ptree pj;
    try
    {
        pj = ptree.get_child("json").get_child(std::to_string(id));
    }
    catch(const boost::property_tree::ptree_error &ec)
    {
        return responseJson(ec.what(), -1);
    }

    boost::property_tree::json_parser::write_json(ss, pj);
    std::cout<< ss.str();
    return responseJson("", 0, true, &pj);


}

std::string CloudSession::readJsonFile()
{

}

// Writing and adding new json to old json
void CloudSession::writeJson(boost::property_tree::ptree json)
{
    std::cout<<"Writing Json"<<std::endl;
    boost::property_tree::ptree oldJson;
    boost::property_tree::read_json("setting.json", oldJson);
    oldJson.get_child("json").put_child(json.front().first, json.front().second);
    boost::property_tree::write_json(std::cout, oldJson);
    boost::property_tree::write_json("setting.json", oldJson);
}

int CloudSession::readId()
{
    std::cout<<"Reading ID"<<std::endl;
    char id[10];
    try
    {
        socket.wait(socket.wait_read);
        size_t size = socket.read_some(boost::asio::buffer((void*)&id, sizeof(id)));
        std::cout<<"Receive request for ID: "<<id<<std::endl;
        return atoi(id);
    }
    catch(boost::system::system_error ec)
    {
        std::cout<<"Error: "<< ec.what()<<" - "<<ec.code() <<std::endl;
        return 0;
//        socket.close();
    }
}

std::string CloudSession::responseJson(std::string errorStr, error_t ec,  bool response, boost::property_tree::ptree *json)
{
    boost::property_tree::ptree pj;
    if(json == nullptr)
        json = new boost::property_tree::ptree;
    pj.add_child("json", *json);
    pj.add("response", response);
    pj.add("error", errorStr);
    pj.add("error_code", ec);
    std::stringstream ss;
    boost::property_tree::write_json(ss, pj);
    return ss.str();
}

boost::asio::ip::tcp::socket *CloudSession::getSocket()
{
    return &socket;
}
