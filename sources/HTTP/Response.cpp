#include "Response.hpp"



Response::Response() {
    sentLength = 0;
    Status = "HTTP/1.1 200 OK\n"
             "Content-Type: text/html\n\n";

}
