#ifndef DATA_H
#define DATA_H

#include <string>

typedef struct data
{
    size_t size;
    error_t error_code;
    std::string dataStr;

    data(std::string _dataStr, error_t ec = 0): error_code(ec), dataStr(_dataStr)
    {
        size = dataStr.size();
    }

    data(){}

}data;

enum REQUEST_TYPE
{
    Read = 'R',
    Write = 'W'
};

#endif // DATA_H
