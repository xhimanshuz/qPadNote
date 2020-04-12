#include "cloudserverengine.h"
#include <boost/asio.hpp>

int main()
{
    auto ioc = new boost::asio::io_context();
    CloudServerEngine cloud(ioc);

    ioc->run();

    return 0;
}
