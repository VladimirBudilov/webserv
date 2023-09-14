#include "../../includes/webserv.hpp"

int main() {
	std::ifstream infile("/Users/anatasha/MyProjects/webserv/webserv.conf");
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

void parseServer(std::vector<std::string> &str, std::vector<Server>& servers, int& i) {
	i++;
	Server res;
	while (str[i] != "}" && i < str.size()) {
		std::stringstream ss(str[i]);
		std::string word;
		ss >> word;
		if (word == "listen") {
			res.listen(ss);
		} else if (word == "port") {
			res.port(ss);
		} else if (word == "server_name") {
			res.server_name(ss);
		} else if (word == "error_page") {
			res.error_page(ss);
		} else if (word == "location") {
			res.parseLocation(str, i);
		} else if (word == "client_max_body_size") {
			res.max_body_size(ss);
		} else {
			std::cout << "Error, bad config file1: " << str[i] << " " << i << std::endl;
			exit(1);
		}
		i++;
	}
	if (str[i] != "}") {
		std::cout << "Error, bad config file2: " << str[i] << std::endl;
		exit(1);
	}
	servers.push_back(res);
}

void removeComments(std::vector<std::string> &str) {
	for (size_t i = 0; i < str.size(); i++) {
		size_t pos = str[i].find('#');
		if (pos != std::string::npos) {
			str[i].erase(pos);
		}
	}
	for (size_t i = 0; i < str.size(); i++) {
		if (str[i].empty()) {
			str.erase(str.begin() + i);
			i--;
		}
	}
}

bool isValidIP(const std::string& ip) {
	std::stringstream ss(ip);
	std::string segment;
	int i = 0, num;
	while (std::getline(ss, segment, '.')) {
		if (segment.empty() || segment.size() > 3 || (segment.size() > 1 && segment[0] == '0'))
			return false;
		for (int j = 0; j < segment.size(); ++j)
		{
			char c = segment[j];
			if (!isdigit(c))
				return false;
		}
		std::istringstream(segment) >> num;
		if (num < 0 || num > 255) return false;
		++i;
	}
	return i == 4 && ss.eof();
}
