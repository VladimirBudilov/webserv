#ifndef WEBSERV_SERVERCONFIG_HPP
#define WEBSERV_SERVERCONFIG_HPP

#include "Location.hpp"
#include <vector>
#include <map>

class ServerConfig {
private:
	int _port;
	std::string _host;
	std::string _server_name;
	std::map<short, std::string> _error_pages;
	std::vector<Location> _locations;
	long long int _max_body_size;
public:
	ServerConfig();

	int getPort() const;
	const std::string &getHost() const;
	const std::string &getServerName() const;
	const std::map<short, std::string> &getErrorPages() const;
	const std::vector<Location> &getLocations() const;
	long long int getMaxBodySize() const;

	void parseLocation(std::vector<std::string> &str, int& i);
	void listen(std::stringstream &ss);
	void port(std::stringstream &ss);
	void server_name(std::stringstream &ss);
	void error_page(std::stringstream &ss);
	void max_body_size(std::stringstream &ss);

};

#endif //WEBSERV_SERVERCONFIG_HPP
