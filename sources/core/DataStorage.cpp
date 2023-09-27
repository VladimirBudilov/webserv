#include "DataStorage.hpp"

std::map<int, std::string> DataStorage::defaultErrorPages;
std::map<std::string, std::string> DataStorage::mimeTypes;

std::string DataStorage::root = DataStorage::getCurrentWorkingDirectory();

std::string DataStorage::getCurrentWorkingDirectory() {
    char buff[FILENAME_MAX];
    getcwd(buff, FILENAME_MAX);
    std::string current_working_dir(buff);
    initDefaultErrorPages();
    return current_working_dir;
}

void DataStorage::initDefaultErrorPages() {
    defaultErrorPages[400] = root + "/www/errors/standardErrors/HTTP400.html";
    defaultErrorPages[401] = root + "/www/errors/standardErrors/HTTP401.html";
    defaultErrorPages[403] = root + "/www/errors/standardErrors/HTTP403.html";
    defaultErrorPages[404] = root + "/www/errors/standardErrors/HTTP404.html";
    defaultErrorPages[405] = root + "/www/errors/standardErrors/HTTP405.html";
    defaultErrorPages[413] = root + "/www/errors/standardErrors/HTTP413.html";
    defaultErrorPages[500] = root + "/www/errors/standardErrors/HTTP500.html";
    defaultErrorPages[501] = root + "/www/errors/standardErrors/HTTP501.html";
    defaultErrorPages[505] = root + "/www/errors/standardErrors/HTTP505.html";
}



