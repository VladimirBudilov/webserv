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
	int port;
	std::string host;
	std::string server_name;
	std::map<short, std::string> error_pages;
	std::vector<Location> locations;
public:
	Server();
	Server(const Server &other);
	Server &operator=(const Server &other);
	~Server();

	int getPort() const;
	const std::string &getHost() const;
	const std::string &getServerName() const;
	std::map<short, std::string> &getErrorPages() const;
	std::vector<Location> &getLocations() const;
	void setPort(int port);
	void setHost(const std::string &host);
	void setServerName(const std::string &serverName);
	void setErrorPages(const std::map<short, std::string> &errorPages);
};

#endif //WEBSERV_SERVER_HPP
