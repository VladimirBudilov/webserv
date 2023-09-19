#ifndef WEBSERV_REQUEST_HPP
#define WEBSERV_REQUEST_HPP

#include "WebServHeaders.hpp"

class Request {
public:
    std::string RequestData;
    std::string Method;

    bool hasCGI();
};


#endif //WEBSERV_REQUEST_HPP
