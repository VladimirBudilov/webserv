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
	std::vector<std::string> index;
	std::string cgi_pass;
	bool autoindex;
	bool file_upload;
	std::vector<bool> methods;
	unsigned long long max_body_size;
public:
	void setPath(const std::string &path);
	void setRoot(const std::string &root);
	void setIndex(const std::string &index);
	void setCgiPass(const std::string &cgiPass);
	void setAutoindex(bool autoindex);
	void setFileUpload(bool fileUpload);
	std::vector<bool> &getMethods();
};


#endif //WEBSERV_LOCATION_HPP
