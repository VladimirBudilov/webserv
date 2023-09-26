#include "DataStorage.hpp"

std::string DataStorage::root = DataStorage::getCurrentWorkingDirectory();

void DataStorage::initDefaultErrorPages() {
    defaultErrorPages[400] = root + "/errors/standardErrors/400.html";
    defaultErrorPages[401] = root + "/errors/standardErrors/401.html";
    defaultErrorPages[403] = root + "/errors/standardErrors/403.html";
    defaultErrorPages[404] = root + "/errors/standardErrors/404.html";
    defaultErrorPages[405] = root + "/errors/standardErrors/405.html";
    defaultErrorPages[406] = root + "/errors/standardErrors/406.html";
    defaultErrorPages[408] = root + "/errors/standardErrors/408.html";
    defaultErrorPages[413] = root + "/errors/standardErrors/413.html";
    defaultErrorPages[414] = root + "/errors/standardErrors/414.html";
    defaultErrorPages[500] = root + "/errors/standardErrors/500.html";
    defaultErrorPages[501] = root + "/errors/standardErrors/501.html";
    defaultErrorPages[505] = root + "/errors/standardErrors/505.html";
}
