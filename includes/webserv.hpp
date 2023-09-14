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
#include "Location.hpp"
#include "Server.hpp"

void removeComments(std::vector<std::string> &str);
void parseServer(std::vector<std::string> &str, std::vector<Server>& servers, int& i);
void parseLocation(std::vector<std::string> &str, std::vector<Location>& locations, int& i);
void listen(std::stringstream &ss, Server &res);
void port(std::stringstream &ss, Server &res);
void server_name(std::stringstream &ss, Server &res);
void error_page(std::stringstream &ss, Server &res);
void root(std::stringstream &ss, Location &res);
void index(std::stringstream &ss, Location &res);
void cgi_pass(std::stringstream &ss, Location &res);
void autoindex(std::stringstream &ss, Location &res);
void file_upload(std::stringstream &ss, Location &res);
void methods(std::stringstream &ss, Location &res);
void max_body_size(std::stringstream &ss, Location &res);
bool isValidIP(std::string ip);

#endif //WEBSERV_WEBSERV_HPP
