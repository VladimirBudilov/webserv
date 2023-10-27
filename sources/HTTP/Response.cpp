#include "Response.hpp"
#include "DataStorage.hpp"

Response::Response() {
    sentLength = 0;
    NumStatus = "HTTP/1.1 200 OK\n";
    ContentType = "Content-Type: ";
    sentLength = 0;
}

void Response::generateDefaultErrorPage(int code) {
    if (code == 404) {
        Status = "HTTP/1.1 404 Not Found\n"
                 "Content-Type: text/html\n\n";
        getDefaultErrorPage(code);
    } else if (code == 405) {
        Status = "HTTP/1.1 405 Method Not Allowed\n"
                 "Content-Type: text/html\n\n";
        getDefaultErrorPage(code);
    } else if (code == 413) {
        Status = "HTTP/1.1 413 Payload Too Large\n"
                 "Content-Type: text/html\n\n";
        getDefaultErrorPage(code);
    } else if (code == 500) {
        Status = "HTTP/1.1 500 Internal Server Error\n"
                 "Content-Type: text/html\n\n";
        getDefaultErrorPage(code);
    } else if (code == 501) {
        Status = "HTTP/1.1 501 Not Implemented\n"
                 "Content-Type: text/html\n\n";
        getDefaultErrorPage(code);
    } else if (code == 505) {
        Status = "HTTP/1.1 505 HTTP Version Not Supported\n"
                 "Content-Type: text/html\n\n";
        getDefaultErrorPage(code);
    }

}

void Response::getDefaultErrorPage(int code) {
    std::string errorRoot = DataStorage::defaultErrorPages[code];
    std::ifstream file(errorRoot.c_str());
    std::string str;
    std::string response;
    if (file.is_open()) {
        while (std::getline(file, str)) {
            response += str + "\n";
        }
        file.close();
    }
    Body = response;
    ResponseData = Status + Body;

}

void Response::GenerateContentType(const std::string& path) {
    std::string res;
    size_t pos = path.find_last_of('.');
    if (pos != std::string::npos)
        res = path.substr(pos + 1);
    else
        res = "txt";
    Status += NumStatus + ContentType + DataStorage::mimeTypes[res] + "\n\n";
}
