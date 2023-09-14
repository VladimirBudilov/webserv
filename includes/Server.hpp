//
// Created by Andy Natasha on 9/11/23.
//

#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

#include "Location.hpp"
#include <vector>
#include <map>

class Server {
private:
	int _port;
	std::string _host;
	std::string _server_name;
	std::map<short, std::string> _error_pages;
	std::vector<Location> _locations;
	unsigned long long _max_body_size;
public:
	Server();
//	Server(const Server &other);
//	Server &operator=(const Server &other);
//	~Server();

//	int getPort() const;
//	const std::string &getHost() const;
//	const std::string &getServerName() const;
//	std::map<short, std::string> &getErrorPages() const;
//	std::vector<Location> &getLocations() const;
//	void setPort(int port);
//	void setHost(const std::string &host);
//	void setServerName(const std::string &serverName);
//	void setErrorPages(const std::map<short, std::string> &errorPages);

	void parseLocation(std::vector<std::string> &str, int& i);
	void listen(std::stringstream &ss);
	void port(std::stringstream &ss);
	void server_name(std::stringstream &ss);
	void error_page(std::stringstream &ss);
	void max_body_size(std::stringstream &ss);

};

#endif //WEBSERV_SERVER_HPP
