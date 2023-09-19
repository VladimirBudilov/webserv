#include "Request.hpp"

bool Request::hasCGI() {
    if(RequestData.find(".py") != std::string::npos)
        return true;
    return false;
}
