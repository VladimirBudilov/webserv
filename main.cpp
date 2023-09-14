#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "webserv.hpp"
#include "Server.hpp"

int main() {
	std::ifstream infile("~/MyProjects/webserv/webserv.conf");
	if (!infile.is_open()) {
		std::cout << "Error, could not open file." << std::endl;
		return 1;
	}
	std::vector<std::string> lines;
	std::string line;
	while (std::getline(infile, line)) {
		lines.push_back(line);
	}
	infile.close();

	removeComments(lines);

	/*for (size_t i = 0; i < lines.size(); i++) {
		std::cout << lines[i] << std::endl;
	}*/

	std::vector<Server> servers;

	for (int i = 0; i < lines.size(); ++i) {
		if (lines[i] == "server {")
			parseServer(lines, servers,i);
		if (!lines[i].empty())
		{
			std::cout << "Error, bad config file: " << lines[i] << std::endl;
			exit(1);
		}
	}

	return 0;
}

void parseServer(std::vector<std::string> &str, std::vector<Server> servers, int& i) {
	i++;
	Server res;
	while (str[i] != "}" && i < str.size()) {
		std::stringstream ss(str[i]);
		std::string word;
		ss >> word;
		if (word == "listen") {
			listen(ss, res);
		} else if (word == "port") {
			port(ss, res);
		} else if (word == "server_name") {
			server_name(ss, res);
		} else if (word == "error_page") {
			error_page(ss, res);
		} else if (word == "location") {
			parseLocation(str, res.getLocations(), i);
		} else {
			std::cout << "Error, bad config file: " << str[i] << std::endl;
			exit(1);
		}
		i++;
	}
	if (str[i] != "}") {
		std::cout << "Error, bad config file: " << str[i] << std::endl;
		exit(1);
	}
	servers.push_back(res);
}

void parseLocation(std::vector<std::string> &str, std::vector<Location> locations, int& i)
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
	res.setPath(path);
	i++;
	while (str[i] != "\t}" && i < str.size()) {
		std::stringstream ss(str[i]);
		std::string word;
		ss >> word;
		if (word == "root") {
			root(ss, res);
		} else if (word == "index") {
			index(ss, res);
		} else if (word == "cgi_pass") {
			cgi_pass(ss, res);
		} else if (word == "autoindex") {
			autoindex(ss, res);
		} else if (word == "file_upload") {
			file_upload(ss, res);
		} else if (word == "methods") {
			methods(ss, res);
		} else if (word == "max_body_size") {
			max_body_size(ss, res);
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

void removeComments(std::vector<std::string> &str) {
	for (size_t i = 0; i < str.size(); i++) {
		size_t pos = str[i].find('#');
		if (pos != std::string::npos) {
			str[i].erase(pos);
		}
	}
}

bool isValidIP(std::string ip) {
	std::stringstream ss(ip);
	std::string segment;
	int i = 0, num;
	while (std::getline(ss, segment, '.')) {
		if (segment.empty() || segment.size() > 3 || (segment.size() > 1 && segment[0] == '0')) return false;
		for (int i = 0; i < segment.size(); ++i)
		{
			char c = segment[i];
			if (!isdigit(c))
				return false;
		}
		std::istringstream(segment) >> num;
		if (num < 0 || num > 255) return false;
		++i;
	}
	return i == 4 && ss.eof();
}

void listen(std::stringstream &ss, Server &res) {
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
	res.setHost(word);
}

void port(std::stringstream& ss, Server& res)
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
	res.setPort(port);
}

void server_name(std::stringstream& ss, Server& res)
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
	res.setServerName(word);
}

void error_page(std::stringstream& ss, Server& res)
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
	res.getErrorPages()[code] = path;
}
