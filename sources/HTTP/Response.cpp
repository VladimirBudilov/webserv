#include "Response.hpp"

Response::Response() {
    sentLength = 0;
    Status = "HTTP/1.1 200 OK\n"
             "Content-Type: text/html\n\n";

}

void Response::generateErrorStatus(int code) {
    if (code == 404) {
        Status = "HTTP/1.1 404 Not Found\n"
                 "Content-Type: text/html\n\n";
    } else if (code == 405) {
        Status = "HTTP/1.1 405 Method Not Allowed\n"
                 "Content-Type: text/html\n\n";
    } else if (code == 413) {
        Status = "HTTP/1.1 413 Payload Too Large\n"
                 "Content-Type: text/html\n\n";
    } else if (code == 500) {
        Status = "HTTP/1.1 500 Internal Server Error\n"
                 "Content-Type: text/html\n\n";
    } else if (code == 501) {
        Status = "HTTP/1.1 501 Not Implemented\n"
                 "Content-Type: text/html\n\n";
    } else if (code == 505) {
        Status = "HTTP/1.1 505 HTTP Version Not Supported\n"
                 "Content-Type: text/html\n\n";
    }

}
