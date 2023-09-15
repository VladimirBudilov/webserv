//
// Created by Andy Natasha on 9/12/23.
//

#include "../../includes/webserv.hpp"

Location::Location() {
	_path = std::string();
	_root = std::string();
	_index =std::string();
	_cgi_pass = std::string();
	_autoindex = false;
	_file_upload = false;
	_methods = std::vector<bool>(2, false);
	_max_body_size = 1024;
}

void Location::setPath(const std::string &path) {
	this->_path = path;
}

void Location::max_body_size(std::stringstream &ss)
{
	std::string word;
	ss >> word;
	if (word[word.size() - 1] != ';')
	{
		std::cout << "Error, bad config file: " << word << std::endl;
		exit(1);
	}
	word.erase(word.size() - 1);
	if (!ss.eof())
	{
		std::cout << "Error, bad config file: " << word << std::endl;
		exit(1);
	}
	for (int i = 0; i < static_cast<int>(word.size()); ++i)
	{
		char c = word[i];
		if (!isdigit(c))
		{
			std::cout << "Error, bad config file: " << word << std::endl;
			exit(1);
		}
	}
	std::stringstream sss(word);
	unsigned long long max_body_size;
	sss >> max_body_size;
	this->_max_body_size = max_body_size;
}

void Location::methods(std::stringstream &ss)
{
	std::string word;
	ss >> word;
	if (word[word.size() - 1] == ';')
	{
		word.erase(word.size() - 1);
		if (word == "GET")
			this->_methods[0] = true;
		else if (word == "POST")
			this->_methods[1] = true;
		else
		{
			std::cout << "Error, bad config file: " << word << std::endl;
			exit(1);
		}
	}
	else
	{
		if (word == "GET")
			this->_methods[0] = true;
		else if (word == "POST")
			this->_methods[1] = true;
		else
		{
			std::cout << "Error, bad config file: " << word << std::endl;
			exit(1);
		}
		ss >> word;
		if (word[word.size() - 1] != ';')
		{
			std::cout << "Error, bad config file: " << word << std::endl;
			exit(1);
		}
		word.erase(word.size() - 1);
		if (word == "GET")
			this->_methods[0] = true;
		else if (word == "POST")
			this->_methods[1] = true;
		else
		{
			std::cout << "Error, bad config file: " << word << std::endl;
			exit(1);
		}
	}
	if (!ss.eof())
	{
		std::cout << "Error, bad config file: " << word << std::endl;
		exit(1);
	}
}


void Location::file_upload(std::stringstream &ss)
{
	std::string word;
	ss >> word;
	if (word[word.size() - 1] != ';')
	{
		std::cout << "Error, bad config file: " << word << std::endl;
		exit(1);
	}
	word.erase(word.size() - 1);
	if (!ss.eof())
	{
		std::cout << "Error, bad config file: " << word << std::endl;
		exit(1);
	}
	if (word == "on")
		this->_file_upload = true;
	else if (word == "off")
		this->_file_upload = false;
	else
	{
		std::cout << "Error, bad config file: " << word << std::endl;
		exit(1);
	}
}

void Location::autoindex(std::stringstream &ss)
{
	std::string word;
	ss >> word;
	if (word[word.size() - 1] != ';')
	{
		std::cout << "Error, bad config file: " << word << std::endl;
		exit(1);
	}
	word.erase(word.size() - 1);
	if (!ss.eof())
	{
		std::cout << "Error, bad config file: " << word << std::endl;
		exit(1);
	}
	if (word == "on")
		this->_autoindex = true;
	else if (word == "off")
		this->_autoindex = false;
	else
	{
		std::cout << "Error, bad config file: " << word << std::endl;
		exit(1);
	}
}

void Location::cgi_pass(std::stringstream &ss)
{
	std::string word;
	ss >> word;
	if (word[word.size() - 1] != ';')
	{
		std::cout << "Error, bad config file: " << word << std::endl;
		exit(1);
	}
	if (!ss.eof())
	{
		std::cout << "Error, bad config file: " << word << std::endl;
		exit(1);
	}
	word.erase(word.size() - 1);
	this->_cgi_pass = word;
}

void Location::index(std::stringstream &ss)
{
	std::string word;
	ss >> word;
	if (word[word.size() - 1] != ';')
	{
		std::cout << "Error, bad config file: " << word << std::endl;
		exit(1);
	}
	if (!ss.eof())
	{
		std::cout << "Error, bad config file: " << word << std::endl;
		exit(1);
	}
	word.erase(word.size() - 1);
	this->_index = word;
}

void Location::root(std::stringstream &ss)
{
	std::string word;
	ss >> word;
	if (word[word.size() - 1] != ';')
	{
		std::cout << "Error, bad config file: " << word << std::endl;
		exit(1);
	}
	word.erase(word.size() - 1);
	this->_root = word;
}
