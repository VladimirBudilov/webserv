#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP

#include "WebServHeaders.hpp"

class Response {

public:
    std::string ResponseData;
    int sentLength;
    std::string Status;
    std::string Body;

    void generateDefoultErrorPage(int code);
    Response();

    void getdefoultErrorPage(int code);
};


#endif //WEBSERV_RESPONSE_HPP
