#include "webserv.hpp"

Location::Location() {
	_path = std::string();
	_root = std::string();
	_index ="index.html";
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
		configError();
	word.erase(word.size() - 1);
	if (!ss.eof())
		configError();
	for (int i = 0; i < static_cast<int>(word.size()); ++i)
	{
		char c = word[i];
		if (!isdigit(c))
			configError();
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
	while (!ss.eof())
	{
		if (word == "GET")
			this->_methods[0] = true;
		else if (word == "POST")
			this->_methods[1] = true;
		else if (word == "DELETE")
			this->_methods[2] = true;
		else
			configError();
		ss >> word;
	}
	if (word[word.size() - 1] != ';')
		configError();
	word.erase(word.size() - 1);
	if (word == "GET")
		this->_methods[0] = true;
	else if (word == "POST")
		this->_methods[1] = true;
	else if (word == "DELETE")
		this->_methods[2] = true;
	else
		configError();
}


void Location::file_upload(std::stringstream &ss)
{
	std::string word;
	ss >> word;
	if (word[word.size() - 1] != ';')
		configError();
	word.erase(word.size() - 1);
	if (!ss.eof())
		configError();
	if (word == "on")
		this->_file_upload = true;
	else if (word == "off")
		this->_file_upload = false;
	else
		configError();
}

void Location::redirect(std::stringstream &ss) {
	std::string word;
	ss >> word;
	if (word[word.size() - 1] != ';')
		configError();
	word.erase(word.size() - 1);
	if (!ss.eof())
		configError();
	this->_redirect_path = word;
	this->_redirect = true;
}

void Location::autoindex(std::stringstream &ss)
{
	std::string word;
	ss >> word;
	if (word[word.size() - 1] != ';')
		configError();
	word.erase(word.size() - 1);
	if (!ss.eof())
		configError();
	if (word == "on")
		this->_autoindex = true;
	else if (word == "off")
		this->_autoindex = false;
	else
		configError();
}

void Location::cgi_pass(std::stringstream &ss)
{
	std::string word;
	ss >> word;
	if (word[word.size() - 1] != ';')
		configError();
	if (!ss.eof())
		configError();
	word.erase(word.size() - 1);
	this->_cgi_pass = word;
}

void Location::index(std::stringstream &ss)
{
	std::string word;
	ss >> word;
	if (word[word.size() - 1] != ';')
		configError();
	if (!ss.eof())
		configError();
	word.erase(word.size() - 1);
	this->_index = word;
}

void Location::root(std::stringstream &ss)
{
	std::string word;
	ss >> word;
	if (word[word.size() - 1] != ';')
		configError();
	word.erase(word.size() - 1);
	this->_root = word;
}

const std::string &Location::getPath() const {
	return _path;
}

const std::string &Location::getRoot() const {
	return _root;
}

const std::string &Location::getIndex() const {
	return _index;
}

const std::string &Location::getCgiPass() const {
	return _cgi_pass;
}

bool Location::isAutoindex() const {
	return _autoindex;
}

bool Location::isFileUpload() const {
	return _file_upload;
}

const std::vector<bool> &Location::getMethods() const {
	return _methods;
}

unsigned long long int Location::getMaxBodySize() const {
	return _max_body_size;
}
