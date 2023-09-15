//
// Created by Andy Natasha on 9/11/23.
//

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
#include "Location.hpp"
#include "Server.hpp"

void removeComments(std::vector<std::string> &str);
void parseServer(std::vector<std::string> &str, std::vector<Server>& servers, int& i);
void configError();
bool isValidIP(const std::string& ip);

#endif //WEBSERV_WEBSERV_HPP
