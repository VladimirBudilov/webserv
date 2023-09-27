#ifndef WEBSERV_WEBSERV_HPP
#define WEBSERV_WEBSERV_HPP

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cstdlib>
#include <cctype>
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <ctime>
#include "Location.hpp"
#include "ServerConfig.hpp"

void removeComments(std::vector<std::string> &str);
void parseServer(std::vector<std::string> &str, std::vector<ServerConfig>& servers, int& i);
void configError();
bool isValidIP(const std::string& ip);
std::string generate_autoindex(const std::string &rootPath, const std::string &location);

#endif //WEBSERV_WEBSERV_HPP
