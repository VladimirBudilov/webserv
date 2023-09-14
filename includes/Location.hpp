//
// Created by Andy Natasha on 9/12/23.
//

#ifndef WEBSERV_LOCATION_HPP
#define WEBSERV_LOCATION_HPP


#include <string>
#include <vector>

class Location {
private:
	std::string _path;
	std::string _root;
	std::string _index;
	std::string _cgi_pass;
	bool _autoindex;
	bool _file_upload;
	std::vector<bool> _methods;
	unsigned long long _max_body_size;
public:
	Location();
//	Location(const Location &other);
//	Location &operator=(const Location &other);
//	~Location();

	void setPath(const std::string &path);
//	void setRoot(const std::string &root);
//	void setIndex(const std::string &index);
//	void setCgiPass(const std::string &cgiPass);
//	void setAutoindex(bool autoindex);
//	void setFileUpload(bool fileUpload);
//	std::vector<bool> &getMethods();
//    void setMaxBodySize(unsigned long long maxBodySize);

	void root(std::stringstream &ss);
	void index(std::stringstream &ss);
	void cgi_pass(std::stringstream &ss);
	void autoindex(std::stringstream &ss);
	void file_upload(std::stringstream &ss);
	void methods(std::stringstream &ss);
	void max_body_size(std::stringstream &ss);

};

#endif //WEBSERV_LOCATION_HPP
