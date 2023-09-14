//
// Created by Andy Natasha on 9/11/23.
//

#include "../../includes/webserv.hpp"

Server::Server() {
	this->_port = 80;
	this->_server_name = "localhost";
	this->_host = "error";
	this->_error_pages = std::map<short, std::string>();
	this->_max_body_size = 1024;
	this->_locations = std::vector<Location>();
}

void Server::parseLocation(std::vector<std::string> &str, int& i)
{
	std::stringstream mss(str[i]);
	std::string location, word, path;
	Location res;
	mss >> location >> path >> word;
	if (location != "location" || word != "{" || mss.eof() != true)
	{
		std::cout << "Error, bad config file: " << str[i] << std::endl;
		exit(1);
	}
	if (path[0] != '/')
	{
		std::cout << "Error, bad config file: " << str[i] << std::endl;
		exit(1);
	}
	res.setPath(path);
	i++;
	while (str[i] != "\t}" && i < str.size()) {
		std::stringstream ss(str[i]);
		std::string word;
		ss >> word;
		if (word == "root") {
			res.root(ss);
		} else if (word == "index") {
			res.index(ss);
		} else if (word == "cgi_pass") {
			res.cgi_pass(ss);
		} else if (word == "autoindex") {
			res.autoindex(ss);
		} else if (word == "file_upload") {
			res.file_upload(ss);
		} else if (word == "methods") {
			res.methods(ss);
		} else if (word == "client_max_body_size") {
			res.max_body_size(ss);
		}
		else {
			std::cout << "Error, bad config file: " << str[i] << std::endl;
			exit(1);
		}
		i++;
	}
	if (str[i] != "\t}") {
		std::cout << "Error, bad config file: " << str[i] << std::endl;
		exit(1);
	}
}

void Server::max_body_size(std::stringstream &ss)
{
	std::string word;
	ss >> word;
	if (word[word.size() - 1] != ';')
	{
		std::cout << "Error, bad config file: " << word << std::endl;
		exit(1);
	}
	word.erase(word.size() - 1);
	if (ss.eof() != true)
	{
		std::cout << "Error, bad config file: " << word << std::endl;
		exit(1);
	}
	for (int i = 0; i < word.size(); ++i)
	{
		char c = word[i];
		if (!isdigit(c))
		{
			std::cout << "Error, bad config file: " << word << std::endl;
			exit(1);
		}
	}
	ss << word;
	unsigned long long max_body_size;
	ss >> max_body_size;
	this->_max_body_size = max_body_size;
}

void Server::listen(std::stringstream &ss)
{
	std::string word;
	ss >> word;
	if(word[word.size() -1] != ';')
	{
		std::cout << "Error, bad config file: " << word << std::endl;
		exit(1);
	}
	word.erase(word.size() - 1);
	if(!isValidIP(word))
	{
		std::cout << "Error, bad config file: " << word << std::endl;
		exit(1);
	}
	if (ss.eof() != true) {
		std::cout << "Error, bad config file: " << word << std::endl;
		exit(1);
	}
	this->_host = word;
}

void Server::port(std::stringstream& ss)
{
	std::string word;
	ss >> word;
	if(!ss.eof())
	{
		std::cout << "Error, bad config file: " << word << std::endl;
		exit(1);
	}
	if (word[word.size() - 1] != ';')
	{
		std::cout << "Error, bad config file: " << word << std::endl;
		exit(1);
	}
	word.erase(word.size() - 1);
	for (int i = 0; i < word.size(); ++i)
	{
		char c = word[i];
		if (!isdigit(c))
		{
			std::cout << "Error, bad config file: " << word << std::endl;
			exit(1);
		}
	}
	int port = std::atoi(word.c_str());
	if (port < 0 || port > 65535)
	{
		std::cout << "Error, bad config file: " << word << std::endl;
		exit(1);
	}
	this->_port = port;
}

void Server::server_name(std::stringstream& ss)
{
	std::string word;
	ss >> word;
	if (word[word.size() - 1] != ';')
	{
		std::cout << "Error, bad config file: " << word << std::endl;
		exit(1);
	}
	word.erase(word.size() - 1);
	if (ss.eof() != true)
	{
		std::cout << "Error, bad config file: " << word << std::endl;
		exit(1);
	}
	this->_server_name = word;
}

void Server::error_page(std::stringstream& ss)
{
	std::string word;
	ss >> word;
	for (int i = 0; i < word.size(); ++i)
	{
		char c = word[i];
		if (!isdigit(c))
		{
			std::cout << "Error, bad config file: " << word << std::endl;
			exit(1);
		}
	}
	int code = std::atoi(word.c_str());
	if (code < 100 || code > 599)
	{
		std::cout << "Error, bad config file: " << word << std::endl;
		exit(1);
	}
	std::string path;
	ss >> path;
	if (!ss.eof())
	{
		std::cout << "Error, bad config file: " << path << std::endl;
		exit(1);
	}
	if(path[path.size() - 1] != ';')
	{
		std::cout << "Error, bad config file: " << path << std::endl;
		exit(1);
	}
	path.erase(path.size() - 1);
	std::ifstream infile(path);
	if (!infile.is_open())
	{
		std::cout << "Error, bad config file: " << path << std::endl;
		exit(1);
	}
	infile.close();
	this->_error_pages[code] = path;
}