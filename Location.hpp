//
// Created by Andy Natasha on 9/12/23.
//

#ifndef WEBSERV_LOCATION_HPP
#define WEBSERV_LOCATION_HPP


#include <string>
#include <vector>

class Location {
private:
	std::string path;
	std::string root;
	std::string index;
	std::string cgi_pass;
	bool autoindex;
	bool file_upload;
	std::vector<bool> methods;
	unsigned long long max_body_size;
public:
	void setPath(const std::string &path);
};


#endif //WEBSERV_LOCATION_HPP
