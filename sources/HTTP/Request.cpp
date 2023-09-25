#include "Request.hpp"

bool Request::hasCGI() {
    //TODO add more validation
    if(RequestData.find(".py") != std::string::npos)
        return true;
    return false;
}

void Request::parse_request(const std::string &request) {
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

const std::string &Request::getMethod() const {
    return method;
}

const std::string &Request::getPath() const {
    return path;
}

bool Request::isVersion() const {
    return version;
}

const std::map<std::string, std::string> &Request::getHeaders() const {
    return headers;
}

const std::string &Request::getBody() const {
    return body;
}