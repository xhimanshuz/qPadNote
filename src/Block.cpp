#include "Block.h"

using namespace Protocol;

Block::Block(bsoncxx::document::view block)
{
    fillData(block);
}

Block::Block(int64_t _id, std::string _tid, const int32_t _uid, bool _isDone, const std::string _title, const std::string _substring): _id(_id),
   isDone(_isDone), uid(_uid)
{
    std::strcpy(tid, _tid.c_str());
    std::strcpy(title, _title.c_str());
    std::strcpy(substring, _substring.c_str());
}

Block::Block()
{
    _id = 0;
    isDone = false;
    memset(&tid, '\0', 10);
    memset(&title, '\0', 12);
    memset(&substring, '\0', 12);
    memset(&substring, '\0', 64);
    uid = 0;
}

Block::~Block()
{

}

bsoncxx::document::value Block::toDocumentValue()
{
    auto blockValue = bsoncxx::builder::stream::document{}
            << "_id" << _id
            << "tid" << tid
            << "uid" << uid
            << "isDone" << isDone
            << "title" << title
            << "substring" << substring
            << bsoncxx::builder::stream::finalize;
    return blockValue;
}

bool Block::fillData(bsoncxx::document::view block)
{
//        try
//        {
        // FILL _id
        _id = block["_id"].get_value().get_int64();

        isDone = block["isDone"].get_value().get_bool();

        auto tempStr = block["tid"].get_utf8().value.to_string();
        strcpy(tid, tempStr.c_str());

        tempStr = block["title"].get_utf8().value.to_string();
        strcpy(title, tempStr.c_str());

        tempStr = block["substring"].get_utf8().value.to_string();
        strcpy(substring, tempStr.c_str());

        uid = block["uid"].get_value().get_int32().value;
//
//        catch(...)
//        {
//            std::cout<<"[E] Error in Filling Block Data.\n";
//            return false;
//        }
        return true;
}

const std::string Block::toString()
{
    std::stringstream ss;
    ss << "<<==================>>\n"
       << "_id: " << _id << "\n"
       << "uid" << uid << "\n"
       << "isDone: "<< isDone <<"\n"
       << "tid: " << tid << "\n"
       << "title: "<< title << "\n"
       << "substring: "<< substring << "\n";
    return ss.str();
}

const std::string Block::toJson()
{
    boost::property_tree::ptree json;
    json.put("_id", _id);
    json.put("uid", uid);
    json.put("isDone", isDone);
    json.put("tid", tid);
    json.put("title", title);
    json.put("substring", substring);

    std::stringstream ss;
    boost::property_tree::write_json(ss, json);

    return ss.str();
}





