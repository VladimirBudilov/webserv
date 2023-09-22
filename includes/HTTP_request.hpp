//
// Created by Andy Natasha on 9/22/23.
//

#ifndef WEBSERV_HTTP_REQUEST_HPP
#define WEBSERV_HTTP_REQUEST_HPP

#include "webserv.hpp"

class HTTP_request {
private:
	std::string method;
	std::string path;
	bool version;
	std::map<std::string, std::string> headers;
	std::string body;
public:
	void parse_request(std::string request);
};

#endif //WEBSERV_HTTP_REQUEST_HPP
