#include "Request.hpp"

bool Request::hasCGI() {
    if (RequestData.find(".py") != std::string::npos)
        return true;
    return false;
}

void Request::parse_request(const std::string &request) {
    if (request.find("\r\n") == std::string::npos) {
        return;
    }
    std::string line = request.substr(0, request.find("\r\n"));
    std::stringstream ss(line);
    std::string ver;
    ss >> this->method >> this->path >> ver;
    if (ver == "HTTP/1.1") {
        this->version = true;
    } else
        this->version = false;

    if (this->path.find("?") != std::string::npos) {
        if (this->path.find("?") == this->path.size() - 1 || this->path.find("?") == 0) {
            std::cout << "bad request(?)" << std::endl;
            return;
        }

        std::string arguments = this->path.substr(this->path.find("?") + 1);
        this->path = this->path.substr(0, this->path.find("?"));
        while (arguments.find("&") != std::string::npos) {
            std::string arg = arguments.substr(0, arguments.find("&"));
            this->args.insert(std::make_pair(arg.substr(0, arg.find("=")), arg.substr(arg.find("=") + 1)));
            arguments = arguments.substr(arguments.find("&") + 1);
        }
        this->args.insert(
                std::make_pair(arguments.substr(0, arguments.find("=")), arguments.substr(arguments.find("=") + 1)));
    }

    if (request.find("\r\n\r\n") == std::string::npos) {
        std::cout << "request dont have end" << std::endl;
        return;
    }

    size_t pos = request.find("\r\n\r\n");
    size_t i = 0;
    while (i < pos) {
        if (request.find("\r\n") == std::string::npos) {
            std::cout << "bad request(\"\\r\\n\")" << std::endl;
            return;
        }
        line = request.substr(i, request.find("\r\n", i) - i);
        i += line.size() + 2;
        size_t p = line.find(": ");
        if (p != std::string::npos) {
            this->headers.insert(std::make_pair(line.substr(0, p), line.substr(p + 2)));
        }
    }
    if (method != "GET" && method != "POST" && method != "DELETE") {
        hasError = true;
        Error = 501;
        std::cout << "bad request(method)" << std::endl;
    }
    if (method == "POST")
        this->body = request.substr(pos + 4);
    if (method == "POST" && headers["Transfer-Encoding"] == "chunked") {
        int chunk_size = 0;
        std::string chunk;
        std::string bd = this->body;
        this->body.clear();
        chunk_size = std::atoi(bd.substr(0, bd.find("\r\n")).c_str());
        while (chunk_size != 0) {
            chunk = bd.substr(bd.find("\r\n") + 2, chunk_size);
            this->body += chunk;
            bd = bd.substr(bd.find("\r\n") + 2 + chunk_size + 2);
            chunk_size = std::atoi(bd.substr(0, bd.find("\r\n")).c_str());
        }
    }
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

const std::map<std::string, std::string> &Request::getArgs() const {
    return args;
}

Request::Request() {
    this->Error = 0;
    this->hasError = false;
}
