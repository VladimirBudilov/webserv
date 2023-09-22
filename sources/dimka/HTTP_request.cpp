//
// Created by Andy Natasha on 9/22/23.
//

#include "HTTP_request.hpp"

void HTTP_request::parse_request(std::string request) {
	std::string line = request.substr(0, request.find("\r\n"));
	std::stringstream ss(line);
	std::string ver;
	ss >> this->method >> this->path >> ver;
	if (ver == "HTTP/1.1")
		this->version = true;
	else
		this->version = false;
	size_t pos = request.find("\r\n\r\n");
	size_t i = 0;
	while (i < pos) {
		line = request.substr(i, request.find("\r\n", i) - i);
		i += line.size() + 2;
		size_t p = line.find(": ");
		if (p != std::string::npos)
			this->headers.insert(std::make_pair(line.substr(0, p), line.substr(p + 2)));
	}
	this->body = request.substr(pos + 4);
}


